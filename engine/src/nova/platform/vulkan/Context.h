#pragma once

#include <vector>

#include "Vulkan.h"

#include "nova/core/fwd.h"
#include "nova/math/Size.hpp"

#include "Device.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "CommandBuffer.h"
#include "Fence.h"

#ifndef DEBUG
#define DEBUG
#endif

namespace nova::platform::vulkan {

namespace details {

class DispatcherLoader {
   public:
    explicit DispatcherLoader();

   private:
    vk::DynamicLoader m_dl;
    PFN_vkGetInstanceProcAddr m_vkGetInstanceProcAddr;
};

}  // namespace details

class Context {
   public:
    explicit Context(core::Window& window);
    ~Context();

    const math::Size2u32& getFramebufferSize() const { return m_framebufferSize; }

   private:
    math::Size2u32 m_framebufferSize;

    details::DispatcherLoader m_vulkanDispatcherLoader;
    vk::AllocationCallbacks* m_allocator;

    vk::raii::Context m_context;
    vk::raii::Instance m_instance;

    vk::raii::DebugUtilsMessengerEXT m_debugMessenger;
    vk::raii::SurfaceKHR m_surface;

    Device m_device;
    Swapchain m_swapchain;
    RenderPass m_mainRenderPass;

    std::vector<CommandBuffer> m_graphicsCommandBuffers;

    uint32_t m_imageIndex;
    uint32_t m_currentFrame = 0;

    bool m_recreatingSwapchain;

   public:
    std::vector<vk::raii::Semaphore> imageAvailableSemaphores;
    std::vector<vk::raii::Semaphore> queueCompleteSemaphores;

    std::vector<Fence> inFlightFences;
    std::vector<Fence*> imagesInFlight;
};

}  // namespace nova::platform::vulkan
