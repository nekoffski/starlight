#pragma once

#include <span>

#include "RendererBackend.h"

namespace nova::gfx {

struct RenderPacket {
    std::span<GeometryRenderData> geometries;
};

}  // namespace nova::gfx
