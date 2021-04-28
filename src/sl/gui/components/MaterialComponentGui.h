#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/MaterialComponent.h"

namespace sl::gui::components {

using namespace scene::components;

class MaterialComponentGui : public ComponentGuiImpl<scene::components::MaterialComponent> {
private:
    void renderComponentGuiImpl(MaterialComponent& component, gui::GuiApi& gui,
        asset::AssetManager& assetManager) const override {

        gui.pushId(component.ownerEntityId);

        if (beginComponentTreeNode(gui, "Material", component)) {
            gui.displayText("Ambient color");
            gui.colorPicker3(gui::createHiddenLabel("Ambient color"), component.ambientColor);
            gui.displayText("Diffuse color");
            gui.colorPicker3(gui::createHiddenLabel("Diffuse color"), component.diffuseColor);
            gui.displayText("Specular color");
            gui.colorPicker3(gui::createHiddenLabel("Specular color"), component.specularColor);
            gui.displayText("Shininess");
            gui.dragFloat(gui::createHiddenLabel("rotation"), component.shininess, 0.5f, 0.0f, 128.0f);
            gui.popTreeNode();
        }

        gui.popId();
    }
};

}