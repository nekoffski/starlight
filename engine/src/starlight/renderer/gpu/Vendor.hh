#pragma once

#ifdef SL_USE_VK

#include "vulkan/VKRendererBackend.hh"

namespace sl {
using RendererBackendVendor = vk::VKRendererBackend;
}

#endif
