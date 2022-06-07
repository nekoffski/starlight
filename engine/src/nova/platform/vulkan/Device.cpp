#include "Device.h"

#include <set>
#include <string>

#include <kc/core/Log.h>

#ifndef DEBUG
#define DEBUG
#endif

namespace nova::platform::vulkan {

static const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

static vk::raii::PhysicalDevice&& pickPhysicalDevice(
    vk::raii::PhysicalDevices& devices, vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface
);

static QueueFamilyIndices findQueueFamilies(
    vk::raii::PhysicalDevice& device, vk::raii::SurfaceKHR& surface
);

static vk::raii::Device createLogicalDevice(
    vk::raii::PhysicalDevice& physicalDevice, vk::raii::SurfaceKHR& surface,
    const QueueFamilyIndices& indices
);

static SwapChainSupportDetails querySwapChainSupport(
    vk::raii::PhysicalDevice& device, vk::raii::SurfaceKHR& surface
);

static vk::Format detectDepthFormat(vk::raii::PhysicalDevice& device) {
    std::vector<vk::Format> candidates = {
        vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint};

    std::optional<vk::Format> format;

    auto flags = vk::FormatFeatureFlagBits::eDepthStencilAttachment;

    for (auto& candidate : candidates) {
        auto properties = device.getFormatProperties(candidate);

        if ((properties.linearTilingFeatures & flags) == flags ||
            (properties.optimalTilingFeatures & flags) == flags) {
            format = candidate;
            break;
        }
    }

    ASSERT(format.has_value(), "Could not find format for depth buffer");
    return format.value();
}

Device::Device(vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface)
    : m_physicalDevices(instance)
    , m_physicalDevice(pickPhysicalDevice(m_physicalDevices, instance, surface))
    , m_queueFamilyIndices(findQueueFamilies(m_physicalDevice, surface))
    , m_logicalDevice(createLogicalDevice(m_physicalDevice, surface, m_queueFamilyIndices))
    , m_graphicsQueue(m_logicalDevice.getQueue(*m_queueFamilyIndices.graphicsFamily, 0))
    , m_presentQueue(m_logicalDevice.getQueue(*m_queueFamilyIndices.presentFamily, 0))
    , m_depthFormat(detectDepthFormat(m_physicalDevice)) {
    LOG_TRACE("Vulkan device created");
}

SwapChainSupportDetails Device::getSwapChainSupport(vk::raii::SurfaceKHR& surface) {
    return querySwapChainSupport(m_physicalDevice, surface);
}

// utility functions
static QueueFamilyIndices findQueueFamilies(
    vk::raii::PhysicalDevice& device, vk::raii::SurfaceKHR& surface
) {
    QueueFamilyIndices indices;

    int i = 0;
    for (auto& queueFamily : device.getQueueFamilyProperties()) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) indices.graphicsFamily = i;
        if (device.getSurfaceSupportKHR(i, *surface)) indices.presentFamily = i;
        if (indices.isComplete()) break;

        ++i;
    }

    return indices;
}

static bool checkDeviceExtensionSupport(vk::raii::PhysicalDevice& device) {
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : device.enumerateDeviceExtensionProperties())
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

static SwapChainSupportDetails querySwapChainSupport(
    vk::raii::PhysicalDevice& device, vk::raii::SurfaceKHR& surface
) {
    return SwapChainSupportDetails{
        .capabilities = device.getSurfaceCapabilitiesKHR(*surface),
        .formats      = device.getSurfaceFormatsKHR(*surface),
        .presentModes = device.getSurfacePresentModesKHR(*surface)};
}

static bool isDeviceSuitable(vk::raii::PhysicalDevice& device, vk::raii::SurfaceKHR& surface) {
    QueueFamilyIndices indices = findQueueFamilies(device, surface);

    if (not checkDeviceExtensionSupport(device)) return false;

    const auto swapChainSupport = querySwapChainSupport(device, surface);
    bool swapChainAdequate =
        !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();

    return indices.isComplete() && swapChainAdequate;
}

static vk::raii::PhysicalDevice&& pickPhysicalDevice(
    vk::raii::PhysicalDevices& physicalDevices, vk::raii::Instance& instance,
    vk::raii::SurfaceKHR& surface
) {
    ASSERT(physicalDevices.size() > 0, "Could not find any physical device");

    LOG_DEBUG("Physical devices found:");
    for (auto& device : physicalDevices) LOG_DEBUG("\t{}", device.getProperties().deviceName);

    auto device = std::ranges::find_if(physicalDevices, [&](auto& device) -> bool {
        return isDeviceSuitable(device, surface);
    });

    ASSERT(device != physicalDevices.end(), "Could not find suitable physical device");

    return std::move(*device);
}

static vk::raii::Device createLogicalDevice(
    vk::raii::PhysicalDevice& physicalDevice, vk::raii::SurfaceKHR& surface,
    const QueueFamilyIndices& indices
) {
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo{};

        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures{};
    vk::DeviceCreateInfo createInfo{};

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos    = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount   = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    createInfo.enabledLayerCount = 0;

#ifdef DEBUG

    std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

#endif

    return vk::raii::Device{physicalDevice, createInfo};
}

}  // namespace nova::platform::vulkan