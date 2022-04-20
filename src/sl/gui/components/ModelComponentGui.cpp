#include "ModelComponentGui.h"

#include <kc/core/Log.h>

#include "sl/glob/Globals.h"
#include "sl/gui/GuiStyle.h"

namespace sl::gui::components {

using namespace scene::components;

void ModelComponentGui::renderComponentGuiImpl(ModelComponent& component,
                                               asset::AssetManager& assetManager,
                                               ecs::Entity& entity) {
    with_ID(component.ownerEntityId.c_str()) {
        auto& params = m_params[component.ownerEntityId];

        if (beginComponentTreeNode(ICON_FA_FIGHTER_JET "  Model", component)) {
            with_OpenedTreeNode("Meshes") {
                with_OpenedTreeNode("Added") {
                    with_ID("added") {
                        std::vector<std::string> meshesToRemove;

                        for (auto& mesh : component.meshes) {
                            ImGui::Text("%s", mesh->name.c_str());
                            ImGui::SameLine(ImGui::GetWindowWidth() - 55);
                            ImGui::SetWindowFontScale(0.7f);
                            ImGui::Text(ICON_FA_TIMES);

                            if (ImGui::IsItemClicked()) meshesToRemove.push_back(mesh->getId());

                            ImGui::SetWindowFontScale(1.0f);
                        }

                        for (auto& meshId : meshesToRemove)
                            std::erase_if(component.meshes, [&meshId](auto& element) -> bool {
                                return meshId == element->getId();
                            });
                    }
                }

                with_OpenedTreeNode("Available") {
                    constexpr float sectionHeightRatio = 0.07f;

                    const auto childHeight = ImGui::GetWindowHeight() * sectionHeightRatio;

                    with_Child("Meshes: ", ImVec2(0, childHeight)) {
                        showMeshesSection("Predefined", params, glob::Globals::get().geom->meshes);
                        showMeshesSection("Loaded", params, assetManager.getMeshes().getAll());
                    }

                    if (ImGui::Button("Add mesh to model"))
                        if (auto selectedMesh = params.selectedMesh.lock(); selectedMesh) {
                            component.meshes.push_back(selectedMesh);
                            component.boundingBox->rebuild(component.meshes);
                            params.selectedMesh.reset();
                        }
                }
            }
            ImGui::TreePop();
        }
    }
}

void ModelComponentGui::showMeshesSection(const std::string& header, Params& params,
                                          MeshesMap& meshes) {
    with_OpenedTreeNode(header.c_str()) {
        for (auto& [name, mesh] : meshes) {
            bool isSelected = name == params.selectedMeshName;

            if (isSelected) gui::pushTextColor(gui::selectedEntryColor);

            ImGui::Text("%s", name.c_str());

            if (isSelected) gui::popTextColor();

            if (ImGui::IsItemClicked()) {
                params.selectedMeshName = name;
                params.selectedMesh = mesh;
            }
        }
    }
}

}  // namespace sl::gui::components