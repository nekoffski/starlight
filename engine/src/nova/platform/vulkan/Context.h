#pragma once

#include "Vulkan.h"

#include "nova/core/fwd.h"
#include "nova/core/Config.h"

namespace nova::platform::vulkan {

struct Context {
   public:
    explicit Context(core::Window& window, const core::Config& config);
    ~Context();

    VkAllocationCallbacks* getAllocator() const;
    VkInstance getInstance() const;
    VkSurfaceKHR getSurface() const;

   private:
    VkAllocationCallbacks* m_allocator;
    VkInstance m_instance;
    VkSurfaceKHR m_surface;

#ifdef NV_VK_DEBUG
    VkDebugUtilsMessengerEXT m_debugMessenger;
#endif
};

}  // namespace nova::platform::vulkan
