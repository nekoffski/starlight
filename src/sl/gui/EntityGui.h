#pragma once

#include <typeindex>
#include <vector>

#include "ComponentsGui.h"
#include "sl/asset/AssetManager.h"
#include "sl/ecs/Entity.h"

namespace sl::gui {

class EntityGui {
   public:
    void renderEntityGui(ecs::Entity& entity, asset::AssetManager& assetManager) {
        std::vector<std::type_index> indexesToRemove;
        for (const auto& index : entity.getComponentsIndexes()) {
            auto& component = entity.getComponent(index);

            m_componentsGui.renderComponentGui(index, component, assetManager, entity);

            if (component.shouldBeRemoved) indexesToRemove.push_back(index);
        }

        for (const auto& index : indexesToRemove) entity.removeComponent(index);
    }

   private:
    ComponentsGui m_componentsGui;
};
}  // namespace sl::gui