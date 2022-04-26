#include "MeshRendererComponentGui.h"

#include "sl/gui/Utils.h"

namespace sl::gui::components {

using namespace scene::components;

void MeshRendererComponentGui::renderComponentGuiImpl(
    MeshRendererComponent& component, asset::AssetManager& assetManager, ecs::Entity& entity
) {
    auto& params = m_params[component.ownerEntityId];

    with_ID(component.ownerEntityId.c_str()) {
        if (beginComponentTreeNode(ICON_FA_PENCIL_ALT "  Mesh renderer", component)) {
            ImGui::Text("Polygon rendering mode");
            gui::combo("##polygonRenderingMode", &params.polygonModeItem, m_polygonModeLabels);

            component.polygonMode = getPolygonMode(params.polygonModeItem);

            ImGui::Text("\n");
            ImGui::Checkbox("Is transparent for light", &component.isTransparentForLight);
            ImGui::TreePop();
        }
    }
}

int MeshRendererComponentGui::getPolygonMode(int polygonModeItem) const {
    switch (polygonModeItem) {
        case 0:
            return STARL_FILL;
        case 1:
            return STARL_LINE;
        case 2:
            return STARL_POINT;
    }
    return STARL_FILL;
}

}  // namespace sl::gui::components