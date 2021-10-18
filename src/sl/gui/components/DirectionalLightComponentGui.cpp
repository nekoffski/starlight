#include "DirectionalLightComponentGui.h"

#include "sl/gui/Utils.h"

namespace sl::gui::components {

using namespace scene::components;

void DirectionalLightComponentGui::renderComponentGuiImpl(DirectionalLightComponent& component,
    asset::AssetManager& assetManager, ecs::Entity& entity) {

    with_ID(component.ownerEntityId.c_str()) {
        if (beginComponentTreeNode(ICON_FA_SUN "  Directional light", component)) {
            ImGui::Checkbox("Render direction vector", &component.renderDirection);

            ImGui::Text("Direction");

            if (ImGui::DragFloat3("##dlcDirection", &component.direction[0], 0.01f, -15.0f, 15.0f))
                component.recalculateMatrices();

            ImGui::Text("Color");
            ImGui::ColorPicker3("##dlcColor", &component.color[0]);

            with_OpenedTreeNode("Shadow map") {
                gui::showImage(*component.shadowMap, { 250, 250 }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            }

            ImGui::TreePop();
        }
    }
}
}