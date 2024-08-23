#pragma once

#include "starlight/core/math/Core.h"

#include "gpu/Mesh.h"

#include "Material.h"
#include "PointLight.h"
#include "RenderGraph.h"

namespace sl {

struct RenderEntity {
    Mat4<f32> worldTransform;
    Mesh* mesh;
    Material* material;
};

struct RenderPacket {
    std::vector<RenderEntity> entities;
    std::vector<PointLight> pointLights;
    Rect2<u32> viewport;
};

}  // namespace sl
