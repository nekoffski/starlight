#pragma once

#include <memory>
#include <string>

#include "sl/core/event/Event.h"
#include "sl/core/event/EventBus.h"
#include "sl/ecs/Entity.h"
#include "sl/math/Vector.hpp"

#include "Settings.h"
#include "Widget.h"

namespace editor::gui {

using namespace sl::core;

using EntitiesVector = std::vector<std::shared_ptr<sl::ecs::Entity>>;

class ScenePanel : public Widget {

public:
    explicit ScenePanel(const WidgetPosition& position, EntitiesVector& entities, std::shared_ptr<sl::ecs::Entity>& selectedEntity)
        : m_position(position)
        , m_entities(entities)
        , m_selectedEntity(selectedEntity)
        , m_entityIndex(0) {
    }

    void render(sl::gui::GuiProxy& gui) override {
        gui->beginPanel("Scene entities", m_position.origin, m_position.size);
        if (gui->button("Add entity", 150))
            event::EventBus::emitEvent<event::AddEntityEvent>("Entity" + std::to_string(m_entityIndex++));

        gui->breakLine();
        for (auto& entity : m_entities) {
            if (gui->beginTreeNode(entity->getName(), false)) {
                if (gui->isPreviousWidgetClicked())
                    m_selectedEntity = entity;

                gui->popTreeNode();
            }
            if (gui->isPreviousWidgetClicked())
                m_selectedEntity = entity;
        }
        gui->endPanel();
    }

private:
    WidgetPosition m_position;
    EntitiesVector& m_entities;

    std::shared_ptr<sl::ecs::Entity>& m_selectedEntity;
    int m_entityIndex;
};
}
