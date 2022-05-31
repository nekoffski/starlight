
#include "Vulkan.h"

#include <vector>

#include <GLFW/glfw3.h>
#include <kc/core/Log.h>

namespace nova::platform::glfw {

vk::raii::SurfaceKHR createVulkanSurface(
    vk::raii::Instance& instance, void* windowHandle, vk::AllocationCallbacks* allocator
) {
    LOG_TRACE("Creating Vulkan surface");
    ASSERT(windowHandle != nullptr, "windowHandle==nullptr");

    VkSurfaceKHR surface;
    auto nativeAllocator =
        allocator != nullptr ? reinterpret_cast<VkAllocationCallbacks*>(allocator) : nullptr;

    const auto result = glfwCreateWindowSurface(
        *instance, static_cast<GLFWwindow*>(windowHandle), nullptr, &surface
    );

    ASSERT(result == VK_SUCCESS, "Could not create vulkan surface: {}", result);

    LOG_DEBUG("Created Vulkan surface");
    return vk::raii::SurfaceKHR{instance, surface};
}

std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const auto glfwExtensions   = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    return std::vector<const char*>{glfwExtensions, glfwExtensions + glfwExtensionCount};
}

}  // namespace nova::platform::glfw