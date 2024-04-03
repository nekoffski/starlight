#include "VKDevice.h"

#include <kc/core/Utils.hpp>

namespace sl::vk {

VKDevice::VKDevice(Allocator* allocator, VkInstance instance, VkSurfaceKHR surface) :
    m_allocator(allocator), m_instance(instance), m_surface(surface),
    m_supportsDeviceLocalHostVisible(false) {
    pickPhysicalDevice();
    createLogicalDevice();
    assignQueues();
    createCommandPool();
    detectDepthFormat();
}

VKDevice::~VKDevice() {
    if (m_graphicsCommandPool)
        vkDestroyCommandPool(m_logicalDevice, m_graphicsCommandPool, m_allocator);
    if (m_logicalDevice) vkDestroyDevice(m_logicalDevice, m_allocator);
}

void VKDevice::destroy() {}

VkDevice VKDevice::getLogicalDevice() const { return m_logicalDevice; }

VkPhysicalDevice VKDevice::getGPU() const { return m_physicalDevice; }

const VKDevice::QueueIndices& VKDevice::getQueueIndices() const {
    return m_queueIndices;
}

const VKDevice::SwapchainSupportInfo& VKDevice::getSwapchainSupport() const {
    return m_swapchainSupport;
}

VkFormat VKDevice::getDepthFormat() const { return m_depthFormat; }

const VkPhysicalDeviceProperties& VKDevice::getProperties() const {
    return m_properties;
}

void VKDevice::detectDepthFormat() {
    static std::array<std::pair<VkFormat, u8>, 3> candidates = {
        std::make_pair(VK_FORMAT_D32_SFLOAT, 4),
        std::make_pair(VK_FORMAT_D32_SFLOAT_S8_UINT, 4),
        std::make_pair(VK_FORMAT_D24_UNORM_S8_UINT, 3),
    };

    uint32_t flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

    m_depthFormat = VK_FORMAT_UNDEFINED;

    for (const auto& [format, channelCount] : candidates) {
        VkFormatProperties properties;
        vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &properties);

        if (((properties.linearTilingFeatures & flags) == flags) || ((properties.optimalTilingFeatures & flags) == flags)) {
            m_depthFormat       = format;
            m_depthChannelCount = channelCount;
            break;
        }
    }

    ASSERT(
      m_depthFormat != VK_FORMAT_UNDEFINED, "Could not find device depth format"
    );
}

VKDevice::SwapchainSupportInfo queryDeviceSwapchainSupport(
  VkPhysicalDevice physicalDevice, VkSurfaceKHR surface
) {
    VKDevice::SwapchainSupportInfo result{};

    // Surface capabilities
    VK_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      physicalDevice, surface, &result.capabilities
    ));

    uint32_t count = 0;
    VK_ASSERT(
      vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, 0)
    );

    if (count != 0) {
        result.formats.resize(count);
        VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(
          physicalDevice, surface, &count, result.formats.data()
        ));
    }

    // Present modes
    count = 0;
    VK_ASSERT(
      vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, 0)
    );

    if (count != 0) {
        result.presentModes.resize(count);
        VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(
          physicalDevice, surface, &count, result.presentModes.data()
        ));
    }

    return result;
}

std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device
) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, 0);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
      device, &queueFamilyCount, queueFamilies.data()
    );

    return queueFamilies;
}

// TODO: refactor even more!
std::optional<VKDevice::DeviceInfo> isPhysicalDeviceSuitable(
  VkPhysicalDevice device, VkSurfaceKHR surface,
  const VkPhysicalDeviceProperties& properties,
  const VkPhysicalDeviceFeatures& features,
  const VKDevice::PhysicalDeviceRequirements& requirements
) {
    VKDevice::DeviceInfo result{};

    // Discrete GPU?
    if (requirements.discreteGpu && properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        LOG_INFO("VKDevice is not a discrete GPU, and one is required. Skipping.");
        return {};
    }

    // Look at each queue and see what queues it supports
    uint8_t minTransferScore = 255;
    auto queueFamilies       = getQueueFamilyProperties(device);

    auto checkQueue = [&](int i) {
        uint8_t currentTransferScore = 0;

        // Graphics queue?
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            result.queue.graphics = i;
            ++currentTransferScore;
        }

        // Compute queue?
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            result.queue.compute = i;
            ++currentTransferScore;
        }

        // Transfer queue?
        if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
            // Take the index if it is the current lowest. This increases the
            // liklihood that it is a dedicated transfer queue.
            if (currentTransferScore <= minTransferScore) {
                minTransferScore      = currentTransferScore;
                result.queue.transfer = i;
            }
        }

        // Present queue?
        VkBool32 supportsPresent = false;
        VK_ASSERT(
          vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supportsPresent)
        );

        if (supportsPresent) result.queue.present = i;
    };

    for (uint32_t i = 0; i < queueFamilies.size(); ++i) checkQueue(i);

    auto areQueueRequirementsFulfilled = [&]() -> bool {
        return (!requirements.graphics
                || (requirements.graphics && result.queue.graphics != -1))
               && (!requirements.present || (requirements.present && result.queue.present != -1))
               && (!requirements.compute || (requirements.compute && result.queue.compute != -1))
               && (!requirements.transfer || (requirements.transfer && result.queue.transfer != -1));
    };

    if (not areQueueRequirementsFulfilled()) return {};

    LOG_INFO("VKDevice meets queue requirements.");
    LOG_TRACE("Graphics Family Index: {}", result.queue.graphics);
    LOG_TRACE("Present Family Index:  {}", result.queue.present);
    LOG_TRACE("Transfer Family Index: {}", result.queue.transfer);
    LOG_TRACE("Compute Family Index:  {}", result.queue.compute);

    // Query swapchain support.
    auto swapchainSupport = queryDeviceSwapchainSupport(device, surface);

    if (swapchainSupport.formats.size() < 1 || swapchainSupport.presentModes.size() < 1) {
        LOG_INFO("Required swapchain support not present, skipping device.");
        return {};
    }

    result.swapchainSupport = swapchainSupport;

    // VKDevice extensions.
    if (requirements.deviceExtensionsNames.empty()) return result;

    uint32_t availableExtensionCount = 0;
    std::vector<VkExtensionProperties> available_extensions;

    VK_ASSERT(
      vkEnumerateDeviceExtensionProperties(device, 0, &availableExtensionCount, 0)
    );
    available_extensions.resize(availableExtensionCount);

    VK_ASSERT(vkEnumerateDeviceExtensionProperties(
      device, 0, &availableExtensionCount, available_extensions.data()
    ));

    std::vector<const char*> availableExtensionsNames;
    for (auto& availableExtension : available_extensions)
        availableExtensionsNames.push_back(availableExtension.extensionName);

    for (auto& requiredExtension : requirements.deviceExtensionsNames) {
        if (not kc::core::contains(availableExtensionsNames, requiredExtension)) {
            LOG_INFO("Extension {} not available, skipping", requiredExtension);
            return {};
        }
    }

    // Sampler anisotropy
    if (requirements.samplerAnisotropy && !features.samplerAnisotropy) {
        LOG_INFO("VKDevice does not support samplerAnisotropy, skipping.");
        return {};
    }

    // VKDevice meets all requirements.
    return result;
}

void showDeviceType(const VkPhysicalDeviceProperties& properties) {
    // GPU type, etc.
    switch (properties.deviceType) {
        default:
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            LOG_INFO("GPU type is Unknown.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            LOG_INFO("GPU type is Integrated.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            LOG_INFO("GPU type is Descrete.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            LOG_INFO("GPU type is Virtual.");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            LOG_INFO("GPU type is CPU.");
            break;
    }
}

void showDeviceInfo(
  const VkPhysicalDeviceProperties& properties,
  const VkPhysicalDeviceMemoryProperties& memory
) {
    LOG_INFO("Selected device: '{}'.", properties.deviceName);

    showDeviceType(properties);

    LOG_INFO(
      "GPU Driver version: {}.{}.{}", VK_VERSION_MAJOR(properties.driverVersion),
      VK_VERSION_MINOR(properties.driverVersion),
      VK_VERSION_PATCH(properties.driverVersion)
    );

    // Vulkan API version.
    LOG_INFO(
      "Vulkan API version: {}.{}.{}", VK_VERSION_MAJOR(properties.apiVersion),
      VK_VERSION_MINOR(properties.apiVersion),
      VK_VERSION_PATCH(properties.apiVersion)
    );

    // Memory information
    for (uint32_t j = 0; j < memory.memoryHeapCount; ++j) {
        float memory_size_gib =
          (((float)memory.memoryHeaps[j].size) / 1024.0f / 1024.0f / 1024.0f);
        if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            LOG_INFO("Local GPU memory: {} GiB", memory_size_gib);
        } else {
            LOG_INFO("Shared System memory: {} GiB", memory_size_gib);
        }
    }
}

std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance) {
    uint32_t count = 0;
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &count, 0));

    ASSERT(count > 0, "Could not find any Vulkan Physical VKDevice");

    std::vector<VkPhysicalDevice> devices(count);
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &count, devices.data()));

    return devices;
}

VKDevice::PhysicalDeviceRequirements createDeviceRequirements() {
    // TODO: These requirements should probably be driven by engine configuration.
    VKDevice::PhysicalDeviceRequirements requirements{};
    requirements.graphics = true;
    requirements.present  = true;
    requirements.transfer = true;
    // NOTE: Enable this if compute will be required.
    // requirements.compute = true;
    requirements.samplerAnisotropy = true;
    requirements.discreteGpu       = true;

    requirements.deviceExtensionsNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    return requirements;
}

VKDevice::DeviceProperties getDeviceProperties(VkPhysicalDevice device) {
    VKDevice::DeviceProperties properties;

    vkGetPhysicalDeviceProperties(device, &properties.device);
    vkGetPhysicalDeviceFeatures(device, &properties.features);
    vkGetPhysicalDeviceMemoryProperties(device, &properties.memory);

    return properties;
}

void VKDevice::storeDevice(
  VkPhysicalDevice device, const VKDevice::DeviceProperties& properties,
  const DeviceInfo& info
) {
    m_physicalDevice        = device;
    m_queueIndices.graphics = info.queue.graphics;
    m_queueIndices.present  = info.queue.present;
    m_queueIndices.transfer = info.queue.transfer;
    // NOTE: set compute index here if needed.

    // Keep a copy of properties, features and memory info for later use.
    m_properties       = properties.device;
    m_features         = properties.features;
    m_memory           = properties.memory;
    m_swapchainSupport = info.swapchainSupport;

    VkPhysicalDeviceMemoryProperties memory;
    vkGetPhysicalDeviceMemoryProperties(device, &memory);

    // Check if device supports local/host visible combo
    for (int i = 0; i < memory.memoryTypeCount; ++i) {
        if ((memory.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0 && 
            (memory.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0) {
            m_supportsDeviceLocalHostVisible = true;
            break;
        }
    }
}

void VKDevice::pickPhysicalDevice() {
    auto requirements = createDeviceRequirements();

    for (auto device : getPhysicalDevices(m_instance)) {
        auto properties = getDeviceProperties(device);

        auto deviceInfo = isPhysicalDeviceSuitable(
          device, m_surface, properties.device, properties.features, requirements
        );

        if (deviceInfo) {
            showDeviceInfo(properties.device, properties.memory);
            storeDevice(device, properties, *deviceInfo);
            break;
        }
    }

    ASSERT(m_physicalDevice, "Could not pick device");
    LOG_INFO("Physical device selected.");
}

struct QueuesCreateInfo {
    std::vector<VkDeviceQueueCreateInfo> vulkanHandle;
    std::vector<float> prorities;
};

QueuesCreateInfo createQueuesCreateInfo(const std::vector<uint32_t>& queuesIndices) {
    QueuesCreateInfo queuesCreateInfo;

    auto queuesCount = queuesIndices.size();
    queuesCreateInfo.prorities.reserve(queuesCount);
    queuesCreateInfo.vulkanHandle.reserve(queuesCount);

    for (const auto index : queuesIndices) {
        queuesCreateInfo.prorities.push_back(1.0f);

        VkDeviceQueueCreateInfo info;
        info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        info.queueFamilyIndex = index;
        info.queueCount       = 1;
        info.flags            = 0;
        info.pNext            = 0;
        info.pQueuePriorities = &queuesCreateInfo.prorities.back();

        queuesCreateInfo.vulkanHandle.push_back(info);
    }

    return queuesCreateInfo;
}

std::vector<uint32_t> VKDevice::prepareQueuesIndices() {
    static constexpr uint64_t maximumExpectedQueuesCount = 3;

    std::vector<uint32_t> indices;
    indices.reserve(maximumExpectedQueuesCount);
    indices.push_back(m_queueIndices.graphics);

    if (m_queueIndices.graphics != m_queueIndices.present)
        indices.push_back(m_queueIndices.present);
    if (m_queueIndices.graphics != m_queueIndices.transfer)
        indices.push_back(m_queueIndices.transfer);

    return indices;
}

struct DeviceCreateInfo {
    VkDeviceCreateInfo vulkanHandle;
    VkPhysicalDeviceFeatures deviceFeatures;
    std::vector<const char*> extensionsNames;
};

DeviceCreateInfo createDeviceCreateInfo(const QueuesCreateInfo& queuesCreateInfo) {
    DeviceCreateInfo deviceCreateInfo;

    // Request device features.
    // TODO: should be config driven
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy        = VK_TRUE;  // Request anistrophy

    deviceCreateInfo.deviceFeatures = deviceFeatures;
    deviceCreateInfo.extensionsNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo vulkanHandle   = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    vulkanHandle.queueCreateInfoCount = queuesCreateInfo.vulkanHandle.size();
    vulkanHandle.pQueueCreateInfos    = queuesCreateInfo.vulkanHandle.data();

    vulkanHandle.pEnabledFeatures = &deviceCreateInfo.deviceFeatures;

    vulkanHandle.enabledExtensionCount   = deviceCreateInfo.extensionsNames.size();
    vulkanHandle.ppEnabledExtensionNames = deviceCreateInfo.extensionsNames.data();

    // Deprecated and ignored, so pass nothing.
    vulkanHandle.enabledLayerCount   = 0;
    vulkanHandle.ppEnabledLayerNames = 0;

    deviceCreateInfo.vulkanHandle = vulkanHandle;

    return deviceCreateInfo;
}

void VKDevice::createLogicalDevice() {
    LOG_INFO("Creating logical device...");

    const auto queuesIndices = prepareQueuesIndices();
    const auto indicesCount  = queuesIndices.size();

    const auto queuesCreateInfo = createQueuesCreateInfo(queuesIndices);
    const auto deviceCreateInfo = createDeviceCreateInfo(queuesCreateInfo);

    // Create the device.
    VK_ASSERT(vkCreateDevice(
      m_physicalDevice, &deviceCreateInfo.vulkanHandle, m_allocator, &m_logicalDevice
    ));

    LOG_INFO("Logical device created.");
}

void VKDevice::assignQueues() {
    vkGetDeviceQueue(
      m_logicalDevice, m_queueIndices.graphics, 0, &m_queues.graphics
    );
    vkGetDeviceQueue(m_logicalDevice, m_queueIndices.present, 0, &m_queues.present);
    vkGetDeviceQueue(
      m_logicalDevice, m_queueIndices.transfer, 0, &m_queues.transfer
    );

    LOG_INFO("Queues obtained.");
}

void VKDevice::createCommandPool() {
    VkCommandPoolCreateInfo poolCreateInfo = {
        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO
    };
    poolCreateInfo.queueFamilyIndex = m_queueIndices.graphics;
    poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VK_ASSERT(vkCreateCommandPool(
      m_logicalDevice, &poolCreateInfo, m_allocator, &m_graphicsCommandPool
    ));
}

VkCommandPool VKDevice::getGraphicsCommandPool() const {
    return m_graphicsCommandPool;
}

bool VKDevice::supportsDeviceLocalHostVisible() const {
    return m_supportsDeviceLocalHostVisible;
}

void VKDevice::waitIdle() {
    const auto result = vkDeviceWaitIdle(m_logicalDevice);
    ASSERT(
      isGood(result), "vkDeviceWaitDile failed: {}", getResultString(result, true)
    );
}

const VKDevice::Queues& VKDevice::getQueues() { return m_queues; }

VKDevice::SwapchainSupportInfo VKDevice::queryDeviceSwapchainSupport() {
    return sl::vk::queryDeviceSwapchainSupport(m_physicalDevice, m_surface);
}

u8 VKDevice::getDepthChannelCount() const { return m_depthChannelCount; }

std::optional<int32_t> VKDevice::findMemoryIndex(
  uint32_t typeFilter, uint32_t propertyFlags
) const {
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
        bool isSuitable =
          (typeFilter & (1 << i))
          && (memoryProperties.memoryTypes[i].propertyFlags & propertyFlags
             ) == propertyFlags;

        if (isSuitable) return i;
    }

    LOG_WARN("Unable to find suitable memory type!");
    return {};
}

}  // namespace sl::vk
