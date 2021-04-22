#include "ModelComponent.h"

#include "sl/gui/GuiStyle.h"

#include "imgui/imgui.h"

namespace sl::scene::components {

ModelComponent::ModelComponent() {
    instances.push_back(math::Vec3 { 0.0f, 0.0f, 0.0f });
}

void ModelComponent::onGui(gui::GuiApi& gui, asset::AssetManager& assetManager) {
    gui.pushId(ownerEntityId);
    if (beginComponentTreeNode(gui, ICON_FA_FIGHTER_JET "  Model")) {
        std::vector<std::string> meshesMock = {
            "Plane",
            "Cube",
            "Sphere",
        };

        static std::string selectedMesh = "";
        static std::shared_ptr<geom::Mesh> selectedMeshh = nullptr;

        if (gui.beginTreeNode("Meshes")) {
            if (gui.beginTreeNode("Added")) {
                gui.pushId("added");
                for (auto& mesh : meshes) {
                    if (gui.beginTreeNode(mesh->name)) {
                        if (gui.beginTreeNode("Vertices")) {
                            int i = 0;
                            for (auto& vertex : mesh->vertices) {
                                gui.dragFloat3("##" + mesh->name + std::to_string(i++), vertex.position);
                            }

                            gui.popTreeNode();
                        }

                        gui.popTreeNode();
                    }
                }

                gui.popId();
                gui.popTreeNode();
            }

            if (gui.beginTreeNode("Available")) {
                ImGui::BeginChild("Meshes: ", ImVec2(0, 110));

                if (gui.beginTreeNode("Predefined")) {
                    for (auto& meshName : meshesMock) {
                        if (meshName == selectedMesh)
                            gui.pushTextColor(gui::selectedEntryColor);

                        gui.displayText(meshName);

                        if (meshName == selectedMesh)
                            gui.popColor();

                        if (gui.isPreviousWidgetClicked())
                            selectedMesh = meshName;
                    }
                    gui.popTreeNode();
                }

                for (auto& [assetName, asset] : assetManager.getAssetsByType(asset::AssetType::model)) {
                    if (gui.beginTreeNode(assetName)) {
                        for (auto& mesh : asset->as<asset::ModelAsset>()->model->meshes) {
                            auto& meshName = mesh->name;

                            if (meshName == selectedMesh)
                                gui.pushTextColor(gui::selectedEntryColor);

                            gui.displayText(meshName);

                            if (meshName == selectedMesh)
                                gui.popColor();

                            if (gui.isPreviousWidgetClicked()) {
                                selectedMesh = meshName;
                                selectedMeshh = mesh;
                            }
                        }

                        gui.popTreeNode();
                    }
                }

                gui.endChild();

                if (gui.button("Add mesh to model"))
                    if (selectedMeshh != nullptr) {
                        meshes.push_back(selectedMeshh);
                        selectedMeshh = nullptr;
                    }

                gui.popTreeNode();
            }

            gui.popTreeNode();
        }

        if (gui.beginTreeNode("Instances")) {

            gui.popTreeNode();
        }

        // auto meshes = assetManager.

        gui.popTreeNode();
    }

    gui.popId();
}

void ModelComponent::serialize(core::JsonBuilder& builder) {
    // builder.addField("name", "ModelComponent"s).addField("model-id", modelData.model->id);
}

void ModelComponent::deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
    // auto modelId = componentDescription["model-id"].asUInt();
    // entity->addComponent<ModelComponent>(
    //     assetManager.getAssetById(modelId)->as<asset::ModelAsset>()->model);
}
}
