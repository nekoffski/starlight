#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/RigidBodyComponent.h"

namespace sl::gui::components {

class RigidBodyComponentGui : public ComponentGuiImpl<scene::components::RigidBodyComponent> {
private:
    void renderComponentGuiImpl(scene::components::RigidBodyComponent& component,
        gui::GuiApi& gui, asset::AssetManager& assetManager) override {

        gui.pushId(component.ownerEntityId);

        if (beginComponentTreeNode(gui, ICON_FA_VECTOR_SQUARE " Rigid body", component)) {
            gui.checkbox("Use gravity", component.useGravity);

            if (gui.beginTreeNode("Properties")) {
                gui.dragFloat("Mass", component.mass);
                gui.dragFloat3("Velocity", component.velocity);

                gui.popTreeNode();
            }

            if (gui.beginTreeNode("Collider")) {
                gui.checkbox("Enable collisions", component.enableCollisions);

                if (gui.beginTreeNode("Bounding box")) {
                    gui.checkbox("Render bounding box", component.renderBoundingBox);

                    gui.popTreeNode();
                }

                gui.popTreeNode();
            }

            gui.popTreeNode();
        }

        gui.popId();
    }
};

}