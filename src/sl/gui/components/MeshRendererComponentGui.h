#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/MeshRendererComponent.h"

namespace sl::gui::components {

using namespace scene::components;

class MeshRendererComponentGui : public ComponentGuiImpl<scene::components::MeshRendererComponent> {
private:
    void renderComponentGuiImpl(MeshRendererComponent& component, gui::GuiApi& gui,
        asset::AssetManager& assetManager) const override {

        gui.pushId(component.ownerEntityId);

        if (beginComponentTreeNode(gui, ICON_FA_PENCIL_ALT "  Mesh renderer", component)) {
            static int polygonModeItem = 0;

            auto getPolygonMode = [&]() {
                switch (polygonModeItem) {
                case 0:
                    return STARL_FILL;
                case 1:
                    return STARL_LINE;
                case 2:
                    return STARL_POINT;
                default:
                    return STARL_FILL;
                }
            };

            std::vector<std::string> polygonModeLabels = {
                "Fill", "Lines", "Points"
            };

            gui.displayText("Polygon rendering mode");
            gui.combo("##polygonRenderingMode", polygonModeItem, polygonModeLabels);

            component.polygonMode = getPolygonMode();

            gui.popTreeNode();
        }

        gui.popId();
    }
};

}