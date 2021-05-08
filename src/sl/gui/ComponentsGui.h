#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "components/ComponentGui.h"

namespace sl::gui {

class ComponentsGui {
    using ComponentGuiPtr = std::shared_ptr<components::ComponentGui>;

public:
    explicit ComponentsGui();

    void renderComponentGui(std::type_index index, ecs::Component& component,
        GuiApi& gui, asset::AssetManager& assetManager, ecs::Entity& entity);

private:
    std::unordered_map<std::type_index, ComponentGuiPtr> m_componentsGui;
};

}
