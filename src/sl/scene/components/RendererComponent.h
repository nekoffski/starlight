#pragma once

#include "sl/asset/Asset.h"
#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/graphics/Shader.h"

namespace sl::scene::components {

struct RendererComponent : ecs::Component {
    explicit RendererComponent(std::shared_ptr<graphics::Shader> shader)
        : shader(shader) {
    }

    void serialize(core::JsonBuilder& builder) override {
        builder.addField("name", "RendererComponent"s).addField("shader-id", shader->id);
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        auto shaderId = componentDescription["shader-id"].asUInt();
        entity->addComponent<RendererComponent>(
            assetManager.getAssetById(shaderId)->as<asset::ShaderAsset>()->shader);
    }

    std::shared_ptr<graphics::Shader> shader;
};
}
