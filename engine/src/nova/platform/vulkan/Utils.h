#pragma once

#include <kc/core/Log.h>

#define VK_CHECK(x) ASSERT(x == VK_SUCCESS, "Detected Vulkan error: {}", x)
