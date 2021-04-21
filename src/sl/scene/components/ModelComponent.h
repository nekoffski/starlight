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

    void onGui(gui::GuiApi& gui) override {
        gui.pushId(ownerEntityId);
        if (beginComponentTreeNode(gui, ICON_FA_FIGHTER_JET "  Model")) {
            auto& meshes = modelData.model->meshes;

            if (meshes.size() > 0) {
                if (gui.beginTreeNode("Meshes")) {
                    for (auto& mesh : meshes) {
                        gui.displayText(mesh->name);
                    }

                    gui.popTreeNode();
                }
            }

            // if (gui.beginTreeNode("Meshes:")) {
            //     for (auto& mesh : modelData.model->meshes) {
            //         gui.displayText("Textures:");
            //         for (auto& texture : mesh->textures)
            //             gui.showImage(*texture, { 250, 250 });
            //     }
            //     gui.popTreeNode();
            // }

            gui.popTreeNode();
        }

        gui.popId();
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
