#include "Vulkan.h"

#ifdef USE_GLFW
#ifdef USE_VK

#include <vector>

#include <GLFW/glfw3.h>

#include "starlight/core/utils/Log.h"

#define VK_ASSERT(expr) \
    { ASSERT(expr == VK_SUCCESS, "Vulkan Fatal Error: {}", expr); }

namespace sl::glfw {

VkSurfaceKHR createVulkanSurface(
  VkInstance instance, void* windowHandle, VkAllocationCallbacks* allocator
) {
    VkSurfaceKHR surface;

    LOG_TRACE("Creating Vulkan surface");
    ASSERT(windowHandle != nullptr, "windowHandle==nullptr");

    VK_ASSERT(glfwCreateWindowSurface(
      instance, static_cast<GLFWwindow*>(windowHandle), allocator, &surface
    ));

    return surface;
}

std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount
    );

    return std::vector<const char*>{
        glfwExtensions, glfwExtensions + glfwExtensionCount
    };
}

}  // namespace sl::glfw

#endif
#endif
