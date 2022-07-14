#pragma once

#include <vector>

#include "nova/platform/vulkan/Vulkan.h"

namespace nova::platform::glfw {

void createVulkanSurface(
    VkInstance instance, void* windowHandle, VkAllocationCallbacks* allocator, VkSurfaceKHR* surface
);

std::vector<const char*> getRequiredExtensions();

}  // namespace nova::platform::glfw
