#pragma once

#include "Vulkan.h"

#include <kc/core/Log.h>

namespace nova::platform::vulkan {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

class VulkanDevice {
   public:
    explicit VulkanDevice(vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface);

    vk::raii::Device* getLogicalDevice() { return &m_logicalDevice; }

    vk::raii::PhysicalDevice* getPhysicalDevice() { return &m_physicalDevice; }

    int32_t findMemoryIndex(uint32_t typeFilter, vk::MemoryPropertyFlags propertyFlags) {
        std::optional<int32_t> index;

        auto properties = m_physicalDevice.getMemoryProperties();
        for (uint32_t i = 0; i < properties.memoryTypeCount; ++i) {
            if (typeFilter & (1 << i) &&
                (properties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
                index = i;
                break;
            }
        }

        ASSERT(index.has_value(), "Could not find correct memory index");
        return index.value();
    }

    QueueFamilyIndices getQueueFamilies() const { return m_queueFamilyIndices; }

    SwapChainSupportDetails getSwapChainSupport(vk::raii::SurfaceKHR& surface);

    vk::Format getDepthFormat() { return m_depthFormat; }

   private:
    vk::raii::PhysicalDevices m_physicalDevices;
    vk::raii::PhysicalDevice m_physicalDevice;

    QueueFamilyIndices m_queueFamilyIndices;

    vk::raii::Device m_logicalDevice;

    vk::raii::Queue m_graphicsQueue;
    vk::raii::Queue m_presentQueue;

    vk::Format m_depthFormat;
};

}  // namespace nova::platform::vulkan
