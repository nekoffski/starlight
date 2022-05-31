#pragma once

#include "Vulkan.h"

namespace nova::platform::vulkan {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

class VulkanDevice {
   public:
    explicit VulkanDevice(vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface);

    vk::raii::Device* getLogicalDevice() { return &m_logicalDevice; }

   private:
    vk::raii::PhysicalDevices m_physicalDevices;
    vk::raii::PhysicalDevice m_physicalDevice;

    QueueFamilyIndices m_queueFamilyIndices;

    vk::raii::Device m_logicalDevice;

    vk::raii::Queue m_graphicsQueue;
    vk::raii::Queue m_presentQueue;
};

}  // namespace nova::platform::vulkan
