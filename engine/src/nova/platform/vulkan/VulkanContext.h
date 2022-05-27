#pragma once

#include <vector>

#include "Vulkan.h"
#include "VulkanDevice.h"

#include "nova/core/fwd.h"

#ifndef DEBUG
#define DEBUG
#endif

namespace nova::platform::vulkan {

namespace details {

class VulkanDispatcherLoader {
   public:
    explicit VulkanDispatcherLoader();

   private:
    vk::DynamicLoader m_dl;
    PFN_vkGetInstanceProcAddr m_vkGetInstanceProcAddr;
};

}  // namespace details

class VulkanContext {
   public:
    explicit VulkanContext(core::Window& window);
    ~VulkanContext();

   private:
    details::VulkanDispatcherLoader m_vulkanDispatcherLoader;

    vk::AllocationCallbacks* m_allocator;
    vk::Instance m_instance;
    vk::DebugUtilsMessengerEXT m_debugMessenger;

    vk::SurfaceKHR m_surface;

    VulkanDevice m_device;
};

}  // namespace nova::platform::vulkan
