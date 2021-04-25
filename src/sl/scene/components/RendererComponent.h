#pragma once

#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"

namespace sl::scene::components {

struct RendererComponent : ecs::Component {
    explicit RendererComponent() {
    }

    void serialize(core::JsonBuilder& builder) override {
        builder.addField("name", "RendererComponent"s);
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        auto& component = entity->addComponent<RendererComponent>();
    }
};
}
