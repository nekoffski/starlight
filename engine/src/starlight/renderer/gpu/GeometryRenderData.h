#pragma once

#include "starlight/core/math/Glm.h"
#include "starlight/renderer/Geometry.h"

namespace sl {

struct GeometryRenderData {
    Mat4f model;
    Geometry* geometry;
};

}  // namespace sl