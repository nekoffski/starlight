#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan.hpp>

#include <kc/core/Log.h>

#define VK_ASSERT(expr) \
    { ASSERT(expr == VK_SUCCESS, "Vulkan Fatal Error: {}", expr); }

namespace nova::platform::vulkan {

std::string getResultString(VkResult result, bool extended);

bool isGood(VkResult);

}  // namespace nova::platform::vulkan
