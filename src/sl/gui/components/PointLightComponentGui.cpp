#include "PointLightComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void PointLightComponentGui::renderComponentGuiImpl(
    PointLightComponent& component, asset::AssetManager& assetManager, ecs::Entity& entity
) {
    with_ID(component.ownerEntityId.c_str()) {
        if (beginComponentTreeNode(ICON_FA_LIGHTBULB "  Point light", component)) {
            ImGui::Text("Position");
            ImGui::DragFloat3("##plcPositon", &component.position[0], 0.1f);
            ImGui::Text("Color");
            ImGui::ColorPicker3("##plcColor", &component.color[0]);
            ImGui::Text("Attenuation A");
            ImGui::DragFloat("##plcAttenuationA", &component.attenuationA, 0.001f, 0.0f, 10.0f);
            ImGui::Text("Attenuation B");
            ImGui::DragFloat("##plcAttenuationB", &component.attenuationB, 0.001f, 0.0f, 10.0f);
            ImGui::Text("Attenuation C");
            ImGui::DragFloat("##plcAttenuationC", &component.attenuationC, 0.001f, 0.0f, 10.0f);

            ImGui::TreePop();
        }
    }
}

}  // namespace sl::gui::components