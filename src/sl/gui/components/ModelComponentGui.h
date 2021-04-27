#pragma once

#include "ComponentGui.h"
#include "sl/gui/GuiStyle.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/utils/Globals.h"

namespace sl::gui::components {

using namespace scene::components;

class ModelComponentGui : public ComponentGuiImpl<scene::components::ModelComponent> {
private:
    void renderComponentGuiImpl(ModelComponent& component, gui::GuiApi& gui,
        asset::AssetManager& assetManager) const override {

        gui.pushId(component.ownerEntityId);
        if (component.beginComponentTreeNode(gui, ICON_FA_FIGHTER_JET "  Model")) {
            static std::string selectedMesh = "";
            static std::shared_ptr<geom::Mesh> selectedMeshh = nullptr;

            if (gui.beginTreeNode("Meshes")) {
                if (gui.beginTreeNode("Added")) {
                    gui.pushId("added");
                    for (auto& mesh : component.meshes) {
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
                            component.meshes.push_back(selectedMeshh);
                            selectedMeshh = nullptr;
                        }

                    gui.popTreeNode();
                }

                gui.popTreeNode();
            }

            if (gui.beginTreeNode("Instances")) {
                int i = 0;
                for (auto& instance : component.instances)
                    gui.dragFloat3("##" + std::to_string(i++), instance);

                if (gui.button("Add instance"))
                    component.instances.push_back(math::Vec3 { 0.0f, 0.0f, 0.0f });

                gui.popTreeNode();
            }

            gui.popTreeNode();
        }

        gui.popId();
    }
};

}