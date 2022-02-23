#pragma once

#include "ComponentGui.h"
#include "sl/scene/components/MaterialComponent.h"

namespace sl::gui::components {

class MaterialComponentGui : public ComponentGuiImpl<scene::components::MaterialComponent> {
   private:
    void renderComponentGuiImpl(scene::components::MaterialComponent& component,
                                asset::AssetManager& assetManager, ecs::Entity& entity) override;
};

}  // namespace sl::gui::components