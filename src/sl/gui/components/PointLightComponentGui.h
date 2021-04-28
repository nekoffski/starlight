#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/PointLightComponent.h"

namespace sl::gui::components {

using namespace scene::components;

class PointLightComponentGui : public ComponentGuiImpl<scene::components::PointLightComponent> {
private:
    void renderComponentGuiImpl(PointLightComponent& component, gui::GuiApi& gui,
        asset::AssetManager& assetManager) const override {

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
};

}