#pragma once

#include <span>

#include "gpu/RendererBackend.h"

namespace sl {

struct RenderPacket {
    std::span<GeometryRenderData> geometries;
};

}  // namespace sl
