#pragma once

#include <span>

#include "RendererBackend.h"

namespace sl::gfx {

struct RenderPacket {
    std::span<GeometryRenderData> geometries;
};

}  // namespace sl::gfx
