#pragma once

#include <vector>

#include "starlight/platform/vulkan/Vulkan.h"

namespace sl::platform::glfw {

void createVulkanSurface(
    VkInstance instance, void* windowHandle, VkAllocationCallbacks* allocator, VkSurfaceKHR* surface
);

std::vector<const char*> getRequiredExtensions();

}  // namespace sl::platform::glfw
