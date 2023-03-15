#pragma once

#include <vector>

#include "Geometry.h"

namespace nova::gfx {

struct RenderPacket {
    std::vector<Geometry> geometries;
};

}  // namespace nova::gfx
