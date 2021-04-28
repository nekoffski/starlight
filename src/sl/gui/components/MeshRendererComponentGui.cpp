#include "MeshRendererComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void MeshRendererComponentGui::renderComponentGuiImpl(MeshRendererComponent& component,
    gui::GuiApi& gui, asset::AssetManager& assetManager) {

    auto& params = m_params[component.ownerEntityId];

    gui.pushId(component.ownerEntityId);

    if (beginComponentTreeNode(gui, ICON_FA_PENCIL_ALT "  Mesh renderer", component)) {
        gui.displayText("Polygon rendering mode");
        gui.combo("##polygonRenderingMode", params.polygonModeItem, m_polygonModeLabels);

        component.polygonMode = getPolygonMode(params.polygonModeItem);

        gui.popTreeNode();
    }

    gui.popId();
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

}