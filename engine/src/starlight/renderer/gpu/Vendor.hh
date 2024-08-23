#pragma once

#ifdef SL_USE_VK

#include "vulkan/VKRendererBackend.h"

namespace sl {
using RendererBackendVendor = vk::VKRendererBackend;
}

#endif
