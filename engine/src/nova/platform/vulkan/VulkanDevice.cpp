#include "VulkanDevice.h"

#include <kc/core/Log.h>

namespace nova::platform::vulkan {

static vk::PhysicalDevice createPhysicalDevice(vk::Instance& instance);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

VulkanDevice::VulkanDevice(vk::Instance& instance)
    : m_physicalDevice(createPhysicalDevice(instance)) {}

static vk::PhysicalDevice createPhysicalDevice(vk::Instance& instance) {
    auto physicalDevices = instance.enumeratePhysicalDevices();
    ASSERT(physicalDevices.size() > 0, "Could not find any physical device");

    LOG_DEBUG("Physical devices found:");
    for (auto& device : physicalDevices) LOG_DEBUG("\t{}", device.getProperties().deviceName);

    for (auto& device : physicalDevices) {
        const auto properties       = device.getProperties();
        const auto features         = device.getFeatures();
        const auto memoryProperties = device.getMemoryProperties();

        // auto queueFamilies = vk::get
    }

    return vk::PhysicalDevice{};
}

}  // namespace nova::platform::vulkan