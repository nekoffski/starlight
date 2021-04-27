#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gui::components {

using namespace scene::components;

class TransformComponentGui : public ComponentGuiImpl<scene::components::TransformComponent> {
private:
    void renderComponentGuiImpl(TransformComponent& component, gui::GuiApi& gui,
        asset::AssetManager& assetManager) const override {

        gui.pushId(component.ownerEntityId);

        if (component.beginComponentTreeNode(gui, ICON_FA_STREET_VIEW "  Transform")) {
            int trigerred = 0;

            gui.displayText("Translation");
            trigerred += gui.dragFloat3(gui::createHiddenLabel("translation"), component.position, 0.1f);
            gui.displayText("Rotation");
            trigerred += gui.dragFloat3(gui::createHiddenLabel("rotation"), component.rotation, 0.5f, 0.0f, 360.0f);
            gui.displayText("Scale");
            trigerred += gui.dragFloat3(gui::createHiddenLabel("scale"), component.scale, 0.5f, 0.0f, 360.0f);
            gui.popTreeNode();

            if (trigerred > 0)
                component.recalculateTransformation();
        }

        gui.popId();
    }
};

}