#include "ModelComponent.h"

#include "sl/gui/GuiStyle.h"
#include "sl/utils/Globals.h"

#include "imgui/imgui.h"

namespace sl::scene::components {

ModelComponent::ModelComponent() {
    instances.push_back(math::Vec3 { 0.0f, 0.0f, 0.0f });
}

void ModelComponent::onGui(gui::GuiApi& gui, asset::AssetManager& assetManager) {
    std::vector<geom::Mesh*> meshesToRebuild;

    gui.pushId(ownerEntityId);
    if (beginComponentTreeNode(gui, ICON_FA_FIGHTER_JET "  Model")) {
        static std::string selectedMesh = "";
        static std::shared_ptr<geom::Mesh> selectedMeshh = nullptr;

        if (gui.beginTreeNode("Meshes")) {
            if (gui.beginTreeNode("Added")) {
                gui.pushId("added");
                for (auto& mesh : meshes) {
                    if (gui.beginTreeNode(mesh->name)) {

                        gui.popTreeNode();
                    }
                }

                gui.popId();
                gui.popTreeNode();
            }

            if (gui.beginTreeNode("Available")) {
                ImGui::BeginChild("Meshes: ", ImVec2(0, 110));

                if (gui.beginTreeNode("Predefined")) {
                    for (auto& [name, mesh] : GLOBALS().geom->meshes) {
                        if (name == selectedMesh)
                            gui.pushTextColor(gui::selectedEntryColor);

                        gui.displayText(name);

                        if (name == selectedMesh)
                            gui.popColor();

                        if (gui.isPreviousWidgetClicked()) {
                            selectedMesh = name;
                            selectedMeshh = mesh;
                        }
                    }
                    gui.popTreeNode();
                }

                if (gui.beginTreeNode("Loaded")) {
                    for (auto& [name, mesh] : assetManager.getMeshes().getAll()) {
                        if (name == selectedMesh)
                            gui.pushTextColor(gui::selectedEntryColor);

                        gui.displayText(name);

                        if (name == selectedMesh)
                            gui.popColor();

                        if (gui.isPreviousWidgetClicked()) {
                            selectedMesh = name;
                            selectedMeshh = mesh;
                        }
                    }

                    gui.popTreeNode();
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
            int i = 0;
            for (auto& instance : instances)
                gui.dragFloat3("##" + std::to_string(i++), instance);

            if (gui.button("Add instance"))
                instances.push_back(math::Vec3 { 0.0f, 0.0f, 0.0f });

            gui.popTreeNode();
        }

        gui.popTreeNode();
    }

    gui.popId();

    for (auto& mesh : meshesToRebuild)
        mesh->buildVertexArray();
}

void ModelComponent::serialize(core::JsonBuilder& builder) {
    std::vector<int> meshesIds;
    std::ranges::transform(meshes, std::back_inserter(meshesIds),
        [](auto& mesh) -> int { return mesh->id; });

    builder.addField("name", "ModelComponent"s).addField("meshes-ids", meshesIds);
}

void ModelComponent::deserialize(std::shared_ptr<ecs::Entity> entity, asset::AssetManager& assetManager, Json::Value& componentDescription) {
    auto& component = entity->addComponent<ModelComponent>();
    auto& meshes = assetManager.getMeshes();

    for (auto& meshId : componentDescription["meshes-ids"]) {
        auto id = meshId.asInt();

        auto mesh = [&]() {
            if (meshes.has(id))
                return meshes.getById(id);

            for (auto& mesh : GLOBALS().geom->meshes | std::views::values)
                if (mesh->id == id)
                    return mesh;

            // TODO: handle this case
        }();

        component.meshes.push_back(mesh);
    }
}
}
