#pragma once

#include <span>

#include "gpu/RendererBackend.h"

namespace sl {

struct RenderPacket {
    std::vector<GeometryRenderData> geometries;
    std::vector<GeometryRenderData> uiGeometries;
};

}  // namespace sl
