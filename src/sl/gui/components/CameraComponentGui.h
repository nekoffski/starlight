#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/CameraComponent.h"

namespace sl::gui::components {

class CameraComponentGui : public ComponentGuiImpl<scene::components::CameraComponent> {
private:
    void renderComponentGuiImpl(scene::components::CameraComponent& component,
        asset::AssetManager& assetManager, ecs::Entity& entity) override {

        with_ID(component.ownerEntityId.c_str()) {
            if (beginComponentTreeNode(ICON_FA_CAMERA "  Camera component", component)) {
                ImGui::TreePop();
            }
        }
    }
};

}