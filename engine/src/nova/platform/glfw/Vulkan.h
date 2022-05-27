#pragma once

#include "nova/platform/vulkan/Vulkan.h"

namespace nova::platform::glfw {

vk::SurfaceKHR createVulkanSurface(
    vk::Instance& instance, void* windowHandle, vk::AllocationCallbacks* allocator
);

std::vector<const char*> getRequiredExtensions();

}  // namespace nova::platform::glfw
