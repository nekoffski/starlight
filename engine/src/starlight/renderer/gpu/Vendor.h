#pragma once

#ifdef USE_VK

#include "vulkan/RendererBackend.h"

namespace sl {
using RendererBackendVendor = vk::RendererBackend;
}

#endif
