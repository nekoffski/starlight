#pragma once

#include "Vulkan.h"

#include "starlight/core/fwd.h"
#include "starlight/core/Config.h"

namespace sl::platform::vulkan {

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

}  // namespace sl::platform::vulkan
