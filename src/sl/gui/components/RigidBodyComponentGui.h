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
    void renderComponentGuiImpl(scene::components::RigidBodyComponent& component,
                                asset::AssetManager& assetManager, ecs::Entity& entity) override {
        with_ID(component.ownerEntityId.c_str()) {
            auto& params = m_params[component.ownerEntityId];

            if (beginComponentTreeNode(ICON_FA_VECTOR_SQUARE " Rigid body", component)) {
                ImGui::Checkbox("Use gravity", &component.useGravity);

                with_OpenedTreeNode("Properties") {
                    ImGui::DragFloat("Mass", &component.mass);
                    ImGui::DragFloat3("Velocity", &component.velocity[0]);
                }

                with_OpenedTreeNode("Collider") {
                    ImGui::Checkbox("Enable collisions", &component.enableCollisions);
                    ImGui::Checkbox("Fixed", &component.fixed);

                    if (component.boundingBox != nullptr && params.selectedBoundingBox == 0)
                        params.selectedBoundingBox =
                            kc::core::indexOf(m_boundingBoxes, component.boundingBox->getName(), 0);

                    with_OpenedTreeNode("Bounding box") {
                        if (gui::combo("##Bounding box type", &params.selectedBoundingBox,
                                       m_boundingBoxes))
                            addBoundingBox(component, entity, params);

                        ImGui::Checkbox("Render bounding box frame", &component.renderBoundingBox);
                    }
                }
                ImGui::TreePop();
            }
        }
    }

    void addBoundingBox(scene::components::RigidBodyComponent& component, ecs::Entity& entity,
                        Params& params) {
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

    std::vector<std::string> m_boundingBoxes = {"None", "AABB"};
    std::unordered_map<std::string, Params> m_params;
};

}  // namespace sl::gui::components