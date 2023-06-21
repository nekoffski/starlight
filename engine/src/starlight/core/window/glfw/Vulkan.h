#pragma once
#ifdef USE_GLFW
#ifdef USE_VK

#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace sl::glfw {

void createVulkanSurface(
    VkInstance instance, void* windowHandle, VkAllocationCallbacks* allocator, VkSurfaceKHR* surface
);

std::vector<const char*> getRequiredExtensions();

}  // namespace sl::glfw

#endif
#endif
