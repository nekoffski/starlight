#pragma once

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"

namespace sl::scene::components {

struct MeshRendererComponent : ecs::Component {
    explicit MeshRendererComponent();

    void onGui(gui::GuiApi& gui, asset::AssetManager& assetManager) override;

    void serialize(core::JsonBuilder& builder) override;
    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription);

    std::shared_ptr<gfx::Shader> shader;
    unsigned int polygonMode;
};
}
