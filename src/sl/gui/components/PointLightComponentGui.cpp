#include "PointLightComponentGui.h"

namespace sl::gui::components {

using namespace scene::components;

void PointLightComponentGui::renderComponentGuiImpl(PointLightComponent& component,
    gui::GuiApi& gui, asset::AssetManager& assetManager) {

    gui.pushId(component.ownerEntityId);

    if (beginComponentTreeNode(gui, ICON_FA_LIGHTBULB "  Point light", component)) {
        gui.displayText("Position");
        gui.dragFloat3(gui::createHiddenLabel("plcPositon"), component.position, 0.1f);
        gui.displayText("Color");
        gui.colorPicker3(gui::createHiddenLabel("plcColor"), component.color);
        gui.displayText("Attenuation A");
        gui.dragFloat(gui::createHiddenLabel("plcAttenuationA"), component.attenuationA, 0.001f, 0.0f, 10.0f);
        gui.displayText("Attenuation B");
        gui.dragFloat(gui::createHiddenLabel("plcAttenuationB"), component.attenuationB, 0.001f, 0.0f, 10.0f);
        gui.displayText("Attenuation C");
        gui.dragFloat(gui::createHiddenLabel("plcAttenuationC"), component.attenuationC, 0.001f, 0.0f, 10.0f);

        gui.popTreeNode();
    }

    gui.popId();
}

}