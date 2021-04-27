#pragma once

#include <memory>

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"

namespace sl::scene::components {

struct ModelComponent : ecs::Component {
    explicit ModelComponent();

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription);

    // new one
    std::vector<std::shared_ptr<geom::Mesh>> meshes;
    std::vector<math::Vec3> instances;
};
}
