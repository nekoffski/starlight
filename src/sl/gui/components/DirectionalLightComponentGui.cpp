#include "DirectionalLightComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void DirectionalLightComponentGui::renderComponentGuiImpl(DirectionalLightComponent& component,
    gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) {

    gui.pushId(component.ownerEntityId);

    if (beginComponentTreeNode(gui, ICON_FA_SUN "  Directional light", component)) {
        gui.checkbox("Render direction vector", component.renderDirection);

        gui.displayText("Direction");

        if (gui.dragFloat3(gui::createHiddenLabel("dlcDirection"), component.direction, 0.01f, -15.0f, 15.0f))
            component.recalculateMatrices();

        gui.displayText("Color");
        gui.colorPicker3(gui::createHiddenLabel("dlcColor"), component.color);

        if (gui.beginTreeNode("Shadow map")) {
            gui.showImage(*component.shadowMap, { 250, 250 }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            gui.popTreeNode();
        }

        gui.popTreeNode();
    }

    gui.popId();
}
}