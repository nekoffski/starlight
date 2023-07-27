#pragma once

#include <span>

#include "gpu/RendererBackend.h"

namespace sl {

struct RenderPacket {
    std::span<GeometryRenderData> geometries;
    std::span<GeometryRenderData> uiGeometries;
};

}  // namespace sl
