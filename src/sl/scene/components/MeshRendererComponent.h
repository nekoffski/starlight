#pragma once

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"

namespace sl::scene::components {

struct MeshRendererComponent : ecs::Component {
    explicit MeshRendererComponent();

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription);

    std::shared_ptr<gfx::Shader> shader;
    unsigned int polygonMode;
};
}
