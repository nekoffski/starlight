#pragma once

#include "starlight/core/math/Glm.h"

#include "gpu/Mesh.h"
#include "Material.h"

namespace sl {

struct RenderEntity {
    Mat4f worldTransform;
    Mesh* mesh;
    Material* material;
};

struct RenderPacket {
    std::vector<RenderEntity> entities;
};

}  // namespace sl
