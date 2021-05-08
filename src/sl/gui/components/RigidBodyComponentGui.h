#pragma once

#include "ComponentGui.h"

#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/RigidBodyComponent.h"

#include "sl/physx/AxisAlignedBoundingBox.h"
#include "sl/physx/AxisAlignedCollider.h"

namespace sl::gui::components {

class RigidBodyComponentGui : public ComponentGuiImpl<scene::components::RigidBodyComponent> {
    struct Params {
        int selectedBoundingBox = 0;
    };

private:
    void renderComponentGuiImpl(scene::components::RigidBodyComponent& component,
        gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) override {

        gui.pushId(component.ownerEntityId);

        auto& params = m_params[component.ownerEntityId];

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
                    if (gui.combo("##Bounding box type", params.selectedBoundingBox, m_boundingBoxes))
                        addBoundingBox(component, entity, params);

                    gui.checkbox("Render bounding box frame", component.renderBoundingBox);

                    gui.popTreeNode();
                }
                gui.popTreeNode();
            }
            gui.popTreeNode();
        }

        gui.popId();
    }

    void addBoundingBox(scene::components::RigidBodyComponent& component, ecs::Entity& entity, Params& params) {
        using scene::components::ModelComponent;

        if (not entity.hasComponent<ModelComponent>()) {
            params.selectedBoundingBox = 0;
            return;
        }

        auto& model = entity.getComponent<ModelComponent>();

        component.boundingBox = [&]() -> std::unique_ptr<physx::BoundingBox> {
            switch (params.selectedBoundingBox) {
            case 1:
                return std::make_unique<physx::AxisAlignedBoundingBox>(model.meshes);
            }

            return nullptr;
        }();
    }

    std::vector<std::string> m_boundingBoxes = { "None", "AABB" };
    std::unordered_map<std::string, Params> m_params;
};

}