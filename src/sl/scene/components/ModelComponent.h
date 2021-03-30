#pragma once

#include <memory>

#include "sl/asset/Asset.h"
#include "sl/core/Json.h"
#include "sl/ecs/Component.h"
#include "sl/ecs/Entity.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/data/ModelData.h"

namespace sl::scene::components {

struct ModelComponent : ecs::Component {
    explicit ModelComponent(std::shared_ptr<geom::Model> model)
        : modelData(model) {
        modelData.positions.push_back(math::Vec4(0.0f));
    }

    void serialize(core::JsonBuilder& builder) override {
        builder.addField("name", "ModelComponent"s).addField("model-id", modelData.model->id);
    }

    static void deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
        auto modelId = componentDescription["model-id"].asUInt();
        entity->addComponent<ModelComponent>(
            assetManager.getAssetById(modelId)->as<asset::ModelAsset>()->model);
    }

    gfx::data::ModelData modelData;
};
}
