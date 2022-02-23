#include "MaterialComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void MaterialComponentGui::renderComponentGuiImpl(MaterialComponent& component,
                                                  asset::AssetManager& assetManager,
                                                  ecs::Entity& entity) {
    with_ID(component.ownerEntityId.c_str()) {
        if (beginComponentTreeNode(ICON_FA_MAP " Material", component)) {
            ImGui::Text("Ambient color");
            ImGui::ColorPicker3("#Ambient color", &component.ambientColor[0]);
            ImGui::Text("Diffuse color");
            ImGui::ColorPicker3("#Diffuse color", &component.diffuseColor[0]);
            ImGui::Text("Specular color");
            ImGui::ColorPicker3("#Specular color", &component.specularColor[0]);
            ImGui::Text("Shininess");
            ImGui::DragFloat("#rotation", &component.shininess, 0.5f, 0.0f, 128.0f);

            ImGui::TreePop();
        }
    }
}
}  // namespace sl::gui::components