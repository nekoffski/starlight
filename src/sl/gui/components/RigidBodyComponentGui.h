#pragma once

#include <kc/core/Utils.hpp>

#include "ComponentGui.h"
#include "sl/physx/AxisAlignedBoundingBox.h"
#include "sl/physx/AxisAlignedCollider.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/RigidBodyComponent.h"

namespace sl::gui::components {

class RigidBodyComponentGui : public ComponentGuiImpl<scene::components::RigidBodyComponent> {
    struct Params {
        int selectedBoundingBox = 0;
    };

   private:
    void renderComponentGuiImpl(
        scene::components::RigidBodyComponent& component, asset::AssetManager& assetManager,
        ecs::Entity& entity
    ) override {
        with_ID(component.ownerEntityId.c_str()) {
            auto& params = m_params[component.ownerEntityId];

            if (beginComponentTreeNode(ICON_FA_VECTOR_SQUARE " Rigid body", component)) {
                ImGui::Checkbox("Use gravity", &component.useGravity);

                with_OpenedTreeNode("Properties") {
                    ImGui::DragFloat("Mass", &component.mass);
                    ImGui::DragFloat3("Velocity", &component.velocity[0]);
                    ImGui::Checkbox("Render velocity vector", &component.renderVelocity);
                }

                with_OpenedTreeNode("Collider") {
                    ImGui::Checkbox("Enable collisions", &component.enableCollisions);
                    ImGui::Checkbox("Fixed", &component.fixed);
                }
                ImGui::TreePop();
            }
        }
    }

    std::unordered_map<std::string, Params> m_params;
};

}  // namespace sl::gui::components