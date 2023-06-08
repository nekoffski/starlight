#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include <kc/core/Log.h>
#include <kc/core/ErrorBase.hpp>

#define VK_ASSERT(expr) \
    { ASSERT(expr == VK_SUCCESS, "Vulkan Fatal Error: {}", expr); }

namespace sl::platform::vulkan {

template <typename T> void zeroMemory(T& type) { std::memset(&type, 0, sizeof(T)); }

std::string getResultString(VkResult result, bool extended);

bool isGood(VkResult);

using VkAllocator = VkAllocationCallbacks*;
using Nanoseconds = uint64_t;

}  // namespace sl::platform::vulkan

DEFINE_ERROR(VulkanError);
