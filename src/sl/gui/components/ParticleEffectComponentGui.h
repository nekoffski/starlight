#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/ParticleEffectComponent.h"

namespace sl::gui::components {

class ParticleEffectComponentGui : public ComponentGuiImpl<scene::components::ParticleEffectComponent> {
private:
    void renderComponentGuiImpl(scene::components::ParticleEffectComponent& component,
        gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) override;
};
}