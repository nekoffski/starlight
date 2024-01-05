#pragma once

#include <vector>

#include "starlight/core/math/Transform.h"
#include "gpu/Geometry.h"

namespace sl {

struct Mesh {
    std::vector<Geometry*> geometries;
    Transform transform;
};

}  // namespace sl
