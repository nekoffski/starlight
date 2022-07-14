
#include "Vulkan.h"

#include <vector>

#include <GLFW/glfw3.h>
#include <kc/core/Log.h>

namespace nova::platform::glfw {

void createVulkanSurface(
    VkInstance instance, void* windowHandle, VkAllocationCallbacks* allocator, VkSurfaceKHR* surface
) {
    LOG_TRACE("Creating Vulkan surface");
    ASSERT(windowHandle != nullptr, "windowHandle==nullptr");

    VK_ASSERT(glfwCreateWindowSurface(
        instance, static_cast<GLFWwindow*>(windowHandle), allocator, surface
    ));
}

std::vector<const char*> getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const auto glfwExtensions   = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    return std::vector<const char*>{glfwExtensions, glfwExtensions + glfwExtensionCount};
}

}  // namespace nova::platform::glfw