#pragma once

#include <memory>
#include <string>

#include "sl/ecs/Entity.h"
#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"
#include "sl/math/Vector.hpp"
#include "sl/scene/components/TransformComponent.h"

#include "Settings.h"
#include "Widget.h"

namespace editor::gui {

using namespace sl;

using EntitiesVector = std::vector<std::shared_ptr<sl::ecs::Entity>>;

class ScenePanel : public Widget {

public:
    explicit ScenePanel(const WidgetPosition& position, EntitiesVector& entities, std::shared_ptr<sl::ecs::Entity>& selectedEntity)
        : m_position(position)
        , m_entities(entities)
        , m_selectedEntity(selectedEntity)
        , m_entityIndex(0) {
    }

    void setPosition(const WidgetPosition& position) {
        m_position = position;
    }

    void render(sl::gui::GuiApi& gui) override {
        gui.beginPanel("Scene entities", m_position.origin, m_position.size);
        if (gui.button("Add entity", 150)) {
            event::Emitter::emit<event::AddEntityEvent>("Entity" + std::to_string(m_entityIndex++));
        }

        if (auto scene = m_activeScene.lock(); scene) {
            gui.breakLine();
            for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
                auto onEntityClick = [&]() {
                    m_selectedEntity = entity;

                    using sl::scene::components::TransformComponent;
                    if (entity->hasComponent<TransformComponent>()) {
                        auto& transform = entity->getComponent<TransformComponent>();
                        event::Emitter::emit<event::ChangeSceneCenterEvent>(transform.position);
                    }
                };

                if (gui.beginTreeNode(entity->getName(), false)) {
                    if (gui.isPreviousWidgetClicked())
                        onEntityClick();

                    gui.popTreeNode();
                }
                if (gui.isPreviousWidgetClicked())
                    onEntityClick();
            }
            gui.endPanel();
        }
    }

private:
    WidgetPosition m_position;
    EntitiesVector& m_entities;

    std::shared_ptr<sl::ecs::Entity>& m_selectedEntity;
    int m_entityIndex;
};
}
