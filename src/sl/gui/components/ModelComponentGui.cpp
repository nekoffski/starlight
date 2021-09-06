#include "ModelComponentGui.h"

#include <kc/core/Log.h>
#include "sl/gui/GuiStyle.h"
#include "sl/utils/Globals.h"

namespace sl::gui::components {

using namespace scene::components;

void ModelComponentGui::renderComponentGuiImpl(ModelComponent& component,
    gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) {

    gui.pushId(component.ownerEntityId);
    auto& params = m_params[component.ownerEntityId];

    if (beginComponentTreeNode(gui, ICON_FA_FIGHTER_JET "  Model", component)) {
        if (gui.beginTreeNode("Meshes")) {
            if (gui.beginTreeNode("Added")) {
                gui.pushId("added");

                std::vector<std::string> meshesToRemove;

                for (auto& mesh : component.meshes) {
                    gui.displayText(mesh->name);
                    gui.sameLine(gui.getCurrentWindowWidth() - 55);
                    gui.setFontScale(0.7f);
                    gui.displayText(ICON_FA_TIMES);

                    if (gui.isPreviousWidgetClicked())
                        meshesToRemove.push_back(mesh->getId());

                    gui.setFontScale(1.0f);
                }

                for (auto& meshId : meshesToRemove)
                    std::erase_if(component.meshes,
                        [&meshId](auto& element) -> bool { return meshId == element->getId(); });

                gui.popId();
                gui.popTreeNode();
            }

            if (gui.beginTreeNode("Available")) {
                constexpr float sectionHeightRatio = 0.07f;

                const auto childHeight = math::Vec2 { 0, gui.getCurrentWindowHeight() * sectionHeightRatio };
                gui.beginChild("Meshes: ", childHeight);

                showMeshesSection("Predefined", gui, params, GLOBALS().geom->meshes);
                showMeshesSection("Loaded", gui, params, assetManager.getMeshes().getAll());

                gui.endChild();

                if (gui.button("Add mesh to model"))
                    if (auto selectedMesh = params.selectedMesh.lock(); selectedMesh) {
                        component.meshes.push_back(selectedMesh);
                        params.selectedMesh.reset();
                    }

                gui.popTreeNode();
            }
            gui.popTreeNode();
        }
        gui.popTreeNode();
    }
    gui.popId();
}

void ModelComponentGui::showMeshesSection(const std::string& header, gui::GuiApi& gui, Params& params, MeshesMap& meshes) {
    if (gui.beginTreeNode(header)) {
        for (auto& [name, mesh] : meshes) {
            bool isSelected = name == params.selectedMeshName;

            if (isSelected)
                gui.pushTextColor(gui::selectedEntryColor);

            gui.displayText(name);

            if (isSelected)
                gui.popColor();

            if (gui.isPreviousWidgetClicked()) {
                params.selectedMeshName = name;
                params.selectedMesh = mesh;
            }
        }
        gui.popTreeNode();
    }
}

}