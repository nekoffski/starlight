#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/DirectionalLightComponent.h"

namespace sl::gui::components {

class DirectionalLightComponentGui
    : public ComponentGuiImpl<scene::components::DirectionalLightComponent> {
   private:
    void renderComponentGuiImpl(
        scene::components::DirectionalLightComponent& component, asset::AssetManager& assetManager,
        ecs::Entity& entity
    ) override;
};

}  // namespace sl::gui::components