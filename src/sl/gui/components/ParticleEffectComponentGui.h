#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/ParticleEffectComponent.h"

namespace sl::gui::components {

class ParticleEffectComponentGui
    : public ComponentGuiImpl<scene::components::ParticleEffectComponent> {
    struct Params {
        int selectedTexture = 0;
    };

   private:
    void renderComponentGuiImpl(scene::components::ParticleEffectComponent& component,
                                asset::AssetManager& assetManager, ecs::Entity& entity) override;

    std::unordered_map<std::string, Params> m_params;
};
}  // namespace sl::gui::components