#pragma once

#include "starlight/core/math/Core.hh"

#include "gpu/Mesh.hh"
#include "light/PointLight.hh"
#include "camera/Camera.hh"
#include "Material.hh"

namespace sl {

struct RenderEntity {
    Mat4<f32> worldTransform;
    Mesh* mesh;
    Material* material;
};

struct RenderPacket {
    Camera* camera;
    std::vector<PointLight> pointLights;
    std::vector<RenderEntity> entities;
};

}  // namespace sl
