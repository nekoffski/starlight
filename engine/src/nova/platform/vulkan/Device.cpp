#include "Device.h"

#include <kc/core/Utils.hpp>

namespace nova::platform::vulkan {

struct PhysicalDeviceRequirements {
    bool graphics;
    bool present;
    bool compute;
    bool transfer;
    std::vector<const char*> deviceExtensionsNames;
    bool samplerAnisotropy;
    bool discreteGpu;
};

struct PhysicalDeviceQueueFamilyInfo {
    uint32_t graphics = -1;
    uint32_t present  = -1;
    uint32_t compute  = -1;
    uint32_t transfer = -1;
};

struct DeviceInfo {
    PhysicalDeviceQueueFamilyInfo queue;
    Device::SwapchainSupportInfo swapchainSupport;
};

Device::Device(Context* context) : m_context(context) {
    pickPhysicalDevice();
    createLogicalDevice();
    // getQueues();
    // createCommandPool();
}

Device::~Device() {
    auto allocator = m_context->getAllocator();

    if (m_graphicsCommandPool)
        vkDestroyCommandPool(m_logicalDevice, m_graphicsCommandPool, allocator);
    if (m_logicalDevice) vkDestroyDevice(m_logicalDevice, allocator);
}

Device::SwapchainSupportInfo queryDeviceSwapchainSupport(
    VkPhysicalDevice physicalDevice, VkSurfaceKHR surface
) {
    Device::SwapchainSupportInfo result{};

    // Surface capabilities
    VK_ASSERT(
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &result.capabilities)
    );

    uint32_t count = 0;
    VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, 0));

    if (count != 0) {
        result.formats.resize(count);
        VK_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevice, surface, &count, result.formats.data()
        ));
    }

    // Present modes
    count = 0;
    VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &count, 0));

    if (count != 0) {
        result.presentModes.resize(count);
        VK_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice, surface, &count, result.presentModes.data()
        ));
    }

    return result;
}

std::optional<DeviceInfo> isPhysicalDeviceSuitable(
    VkPhysicalDevice device, VkSurfaceKHR surface, const VkPhysicalDeviceProperties& properties,
    const VkPhysicalDeviceFeatures& features, const PhysicalDeviceRequirements& requirements
) {
    DeviceInfo result{};

    // Discrete GPU?
    if (requirements.discreteGpu) {
        if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            LOG_INFO("Device is not a discrete GPU, and one is required. Skipping.");
            return {};
        }
    }

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, 0);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    // Look at each queue and see what queues it supports
    uint8_t minTransferScore = 255;
    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
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
        VkBool32 supports_present = false;
        VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supports_present));

        if (supports_present) result.queue.present = i;
    }

    if ((!requirements.graphics || (requirements.graphics && result.queue.graphics != -1)) &&
        (!requirements.present || (requirements.present && result.queue.present != -1)) &&
        (!requirements.compute || (requirements.compute && result.queue.compute != -1)) &&
        (!requirements.transfer || (requirements.transfer && result.queue.transfer != -1))) {
        LOG_INFO("Device meets queue requirements.");
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

        // Device extensions.
        if (requirements.deviceExtensionsNames.size() > 0) {
            uint32_t availableExtensionCount = 0;
            std::vector<VkExtensionProperties> available_extensions;

            VK_ASSERT(vkEnumerateDeviceExtensionProperties(device, 0, &availableExtensionCount, 0));

            if (availableExtensionCount != 0) {
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
            }

            // Sampler anisotropy
            if (requirements.samplerAnisotropy && !features.samplerAnisotropy) {
                LOG_INFO("Device does not support samplerAnisotropy, skipping.");
                return {};
            }

            // Device meets all requirements.
            return result;
        }
    }

    return {};
}

bool Device::pickPhysicalDevice() {
    auto instance = m_context->getInstance();
    auto surface  = m_context->getSurface();

    uint32_t physicalDevice_count = 0;
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDevice_count, 0));

    if (physicalDevice_count == 0) {
        LOG_FATAL("No devices which support Vulkan were found.");
        return false;
    }

    VkPhysicalDevice physicalDevices[physicalDevice_count];
    VK_ASSERT(vkEnumeratePhysicalDevices(instance, &physicalDevice_count, physicalDevices));

    for (uint32_t i = 0; i < physicalDevice_count; ++i) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDevices[i], &features);

        VkPhysicalDeviceMemoryProperties memory;
        vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &memory);

        // TODO: These requirements should probably be driven by engine
        // configuration.
        PhysicalDeviceRequirements requirements{};
        requirements.graphics = true;
        requirements.present  = true;
        requirements.transfer = true;
        // NOTE: Enable this if compute will be required.
        // requirements.compute = true;
        requirements.samplerAnisotropy = true;
        requirements.discreteGpu       = true;

        requirements.deviceExtensionsNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        auto deviceInfo = isPhysicalDeviceSuitable(
            physicalDevices[i], surface, properties, features, requirements
        );

        if (deviceInfo) {
            LOG_INFO("Selected device: '{}'.", properties.deviceName);
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

            LOG_INFO(
                "GPU Driver version: {}.{}.{}", VK_VERSION_MAJOR(properties.driverVersion),
                VK_VERSION_MINOR(properties.driverVersion),
                VK_VERSION_PATCH(properties.driverVersion)
            );

            // Vulkan API version.
            LOG_INFO(
                "Vulkan API version: {}.{}.{}", VK_VERSION_MAJOR(properties.apiVersion),
                VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion)
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

            m_physicalDevice     = physicalDevices[i];
            m_graphicsQueueIndex = deviceInfo->queue.graphics;
            m_presentQueueIndex  = deviceInfo->queue.present;
            m_transferQueueIndex = deviceInfo->queue.transfer;
            // NOTE: set compute index here if needed.

            // Keep a copy of properties, features and memory info for later use.
            m_properties       = properties;
            m_features         = features;
            m_memory           = memory;
            m_swapchainSupport = deviceInfo->swapchainSupport;
            break;
        }
    }

    ASSERT(m_physicalDevice, "Could not pick device");
    LOG_INFO("Physical device selected.");

    return true;
}

struct QueuesCreateInfo {
    std::vector<VkDeviceQueueCreateInfo> vulkanHandle;
    std::vector<float> prorities;
};

QueuesCreateInfo getQueuesCreateInfo(const std::vector<uint32_t>& queuesIndices) {
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

std::vector<uint32_t> Device::prepareQueuesIndices() {
    static constexpr uint64_t maximumExpectedQueuesCount = 3;

    std::vector<uint32_t> indices;
    indices.reserve(maximumExpectedQueuesCount);
    indices.push_back(m_graphicsQueueIndex);

    if (m_graphicsQueueIndex != m_presentQueueIndex) indices.push_back(m_presentQueueIndex);
    if (m_graphicsQueueIndex != m_transferQueueIndex) indices.push_back(m_transferQueueIndex);

    return indices;
}

struct DeviceCreateInfo {
    VkDeviceCreateInfo vulkanHandle;
    VkPhysicalDeviceFeatures deviceFeatures;
    std::vector<const char*> extensionsNames;
};

DeviceCreateInfo getDeviceCreateInfo(const QueuesCreateInfo& queuesCreateInfo) {
    DeviceCreateInfo deviceCreateInfo;

    // Request device features.
    // TODO: should be config driven
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy        = VK_TRUE;  // Request anistrophy

    deviceCreateInfo.deviceFeatures = deviceFeatures;
    deviceCreateInfo.extensionsNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

    VkDeviceCreateInfo vulkanHandle   = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
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

void Device::createLogicalDevice() {
    LOG_INFO("Creating logical device...");

    const auto queuesIndices = prepareQueuesIndices();
    const auto indicesCount  = queuesIndices.size();

    const auto queuesCreateInfo = getQueuesCreateInfo(queuesIndices);
    const auto deviceCreateInfo = getDeviceCreateInfo(queuesCreateInfo);

    // Create the device.
    VK_ASSERT(vkCreateDevice(
        m_physicalDevice, &deviceCreateInfo.vulkanHandle, m_context->getAllocator(),
        &m_logicalDevice
    ));

    LOG_INFO("Logical device created.");
}

void Device::getQueues() {
    vkGetDeviceQueue(m_logicalDevice, m_graphicsQueueIndex, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, m_graphicsQueueIndex, 0, &m_presentQueue);
    vkGetDeviceQueue(m_logicalDevice, m_transferQueueIndex, 0, &m_transferQueue);

    LOG_INFO("Queues obtained.");
}

void Device::createCommandPool() {
    VkCommandPoolCreateInfo poolCreateInfo = {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    poolCreateInfo.queueFamilyIndex        = m_graphicsQueueIndex;
    poolCreateInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VK_ASSERT(vkCreateCommandPool(
        m_logicalDevice, &poolCreateInfo, m_context->getAllocator(), &m_graphicsCommandPool
    ));
}

}  // namespace nova::platform::vulkan
