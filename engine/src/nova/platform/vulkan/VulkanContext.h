#pragma once

#include <vector>

#include "Vulkan.h"

#include "nova/core/fwd.h"
#include "nova/math/Size.hpp"

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

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

    const math::Size2u32& getFramebufferSize() const { return m_framebufferSize; }

   private:
    math::Size2u32 m_framebufferSize;

    details::VulkanDispatcherLoader m_vulkanDispatcherLoader;
    vk::AllocationCallbacks* m_allocator;

    vk::raii::Context m_context;
    vk::raii::Instance m_instance;

    vk::raii::DebugUtilsMessengerEXT m_debugMessenger;
    vk::raii::SurfaceKHR m_surface;

    VulkanDevice m_device;
    VulkanSwapchain m_swapchain;

    uint32_t m_imageIndex;
    uint32_t m_currentFrame = 0;

    bool m_recreatingSwapchain;
};

}  // namespace nova::platform::vulkan
