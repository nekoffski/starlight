#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/DirectionalLightComponent.h"

namespace sl::gui::components {

class DirectionalLightComponentGui : public ComponentGuiImpl<scene::components::DirectionalLightComponent> {
private:
    void renderComponentGuiImpl(scene::components::DirectionalLightComponent& component,
        gui::GuiApi& gui, asset::AssetManager& assetManager) override;
};

}