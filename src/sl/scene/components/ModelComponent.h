#pragma once

#include <memory>

#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"

namespace sl::scene::components {

struct ModelComponent : ecs::Component {
    using View = ecs::ComponentView<ModelComponent>;

    explicit ModelComponent() {
        instances.push_back(math::Vec3 { 0.0f, 0.0f, 0.0f });

        name = "ModelComponent";
    }

    std::vector<std::shared_ptr<geom::Mesh>> meshes;
    std::vector<math::Vec3> instances;
};
}
