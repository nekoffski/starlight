#pragma once

#include "starlight/core/math/Core.hh"

#include "gpu/Mesh.hh"
#include "camera/Camera.hh"
#include "Material.hh"
#include "PointLight.hh"

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
    Camera* camera;
};

}  // namespace sl
