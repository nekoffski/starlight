#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/PointLightComponent.h"

namespace sl::gui::components {

class PointLightComponentGui : public ComponentGuiImpl<scene::components::PointLightComponent> {
private:
    void renderComponentGuiImpl(scene::components::PointLightComponent& component, gui::GuiApi& gui,
        asset::AssetManager& assetManager, ecs::Entity& entity) override;
};

}