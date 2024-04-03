#pragma once
#ifdef USE_GLFW
#ifdef USE_VK

#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace sl::glfw {

VkSurfaceKHR createVulkanSurface(
  VkInstance instance, void* windowHandle, VkAllocationCallbacks* allocator
);

std::vector<const char*> getRequiredExtensions();

}  // namespace sl::glfw

#endif
#endif
