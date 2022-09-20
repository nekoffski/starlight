#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include <kc/core/Log.h>
#include <kc/core/ErrorBase.hpp>

#define VK_ASSERT(expr) \
    { ASSERT(expr == VK_SUCCESS, "Vulkan Fatal Error: {}", expr); }

namespace nova::platform::vulkan {

std::string getResultString(VkResult result, bool extended);

bool isGood(VkResult);

using VkAllocator = VkAllocationCallbacks*;
using Nanoseconds = uint64_t;

}  // namespace nova::platform::vulkan

DEFINE_ERROR(VulkanError);
