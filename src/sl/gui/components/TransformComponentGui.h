#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gui::components {

class TransformComponentGui : public ComponentGuiImpl<scene::components::TransformComponent> {
   private:
    void renderComponentGuiImpl(scene::components::TransformComponent& component,
                                asset::AssetManager& assetManager, ecs::Entity& entity) override;

    int m_trigerred = 0;
};

}  // namespace sl::gui::components