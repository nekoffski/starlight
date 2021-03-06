#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gui::components {

class TransformComponentGui : public ComponentGuiImpl<scene::components::TransformComponent> {
private:
    void renderComponentGuiImpl(scene::components::TransformComponent& component,
        gui::GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity) override;

    int m_trigerred = 0;
};

}