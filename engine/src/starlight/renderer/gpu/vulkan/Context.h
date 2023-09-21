#pragma once

#include "Vulkan.h"

#include "starlight/core/fwd.h"
#include "starlight/core/Config.h"

namespace sl::vk {

struct Context {
public:
    explicit Context(sl::Window& window, const Config& config);
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

}  // namespace sl::vk
