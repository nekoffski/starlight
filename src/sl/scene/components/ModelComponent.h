#pragma once

#include <memory>

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"

namespace sl::scene::components {

struct ModelComponent : ecs::Component {
    explicit ModelComponent() {
        instances.push_back(math::Vec3 { 0.0f, 0.0f, 0.0f });
    }

    std::vector<std::shared_ptr<geom::Mesh>> meshes;
    std::vector<math::Vec3> instances;
};
}
