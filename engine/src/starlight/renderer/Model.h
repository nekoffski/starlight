#pragma once

#include <vector>

#include "starlight/core/math/Transform.h"
#include "gpu/Mesh.h"

namespace sl {

struct Model {
    explicit Model();

    Mat4f getModelMatrix();

    std::vector<Mesh*> meshes;
    Transform* transform;
};

}  // namespace sl
