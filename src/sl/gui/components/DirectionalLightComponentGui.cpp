#include "DirectionalLightComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void DirectionalLightComponentGui::renderComponentGuiImpl(DirectionalLightComponent& component,
    gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) {

    gui.pushId(component.ownerEntityId);

    if (beginComponentTreeNode(gui, ICON_FA_SUN "  Directional light", component)) {
        gui.displayText("Direction");

        if (gui.dragFloat3(gui::createHiddenLabel("dlcDirection"), component.direction, 0.01f, -15.0f, 15.0f)) {
            component.viewMatrix = math::lookAt(component.direction, math::Vec3 { 0.0f }, math::Vec3 { 0.0f, 1.0f, 0.0f });
            component.spaceMatrix = lightProjectionMatrix * component.viewMatrix;
        }

        gui.displayText("Color");
        gui.colorPicker3(gui::createHiddenLabel("dlcColor"), component.color);

        if (gui.beginTreeNode("Shadow map"), false) {
            gui.showImage(*component.shadowMap, { 250, 250 });
            gui.popTreeNode();
        }

        gui.popTreeNode();

        gui.checkbox("Render direction vector", component.renderDirection);
    }

    gui.popId();
}
}