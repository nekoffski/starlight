#pragma once

#ifdef USE_VK

#include "vulkan/VKRendererBackend.h"

namespace sl {
using RendererBackendVendor = vk::VKRendererBackend;
}

#endif
