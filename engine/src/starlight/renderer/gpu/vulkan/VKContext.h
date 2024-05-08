#pragma once

#include "Vulkan.h"

#include "starlight/core/fwd.h"
#include "starlight/core/Core.h"
#include "starlight/core/Config.h"
#include "starlight/core/RAIIWrapper.hpp"

namespace sl::vk {

class VKContext : public NonCopyable, public NonMovable {
public:
    explicit VKContext(sl::Window& window, const Config& config);
    ~VKContext();

    VkAllocationCallbacks* getAllocator() const;  // TODO: not const!
    VkInstance getInstance() const;
    VkSurfaceKHR getSurface() const;

private:
    VkInstance createInstance();
    VkDebugUtilsMessengerEXT createDebugMessenger();

    Config m_config;

    Allocator* m_allocator;
    RAIIWrapper<VkInstance> m_instance;
    RAIIWrapper<VkSurfaceKHR> m_surface;
    RAIIWrapper<VkDebugUtilsMessengerEXT> m_debugMessenger;
};

}  // namespace sl::vk
