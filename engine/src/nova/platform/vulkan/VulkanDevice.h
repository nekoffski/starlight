#pragma once

#include "Vulkan.h"

namespace nova::platform::vulkan {

class VulkanDevice {
   public:
    explicit VulkanDevice(vk::Instance& instance);

   private:
    vk::PhysicalDevice m_physicalDevice;
    vk::Device m_logicalDevice;
};

}  // namespace nova::platform::vulkan
