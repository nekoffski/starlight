#pragma once

#include <vector>

#include "starlight/core/math/Transform.h"
#include "gpu/Geometry.h"

namespace sl {

struct Model {
    explicit Model();

    Mat4f getModelMatrix();

    std::vector<Geometry*> geometries;
    Transform* transform;
};

}  // namespace sl
