#pragma once

#include <memory>
#include <string>

#include "sl/ecs/Entity.h"
#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"
#include "sl/scene/components/TransformComponent.h"

#include "editor/DebugConsole.hpp"

#include "Settings.h"
#include "Widget.h"

namespace editor::gui {

using namespace sl;

class ScenePanel : public Widget {
public:
    explicit ScenePanel(const WidgetPosition& position, std::shared_ptr<sl::ecs::Entity>& selectedEntity)
        : m_position(position)
        , m_selectedEntity(selectedEntity)
        , m_entityIndex(0) {
    }

    void setPosition(const WidgetPosition& position) {
        m_position = position;
    }

    void render(sl::gui::GuiApi& gui) override {
        gui.beginPanel("Scene entities", m_position.origin, m_position.size);
        if (gui.button("Add entity", 150))
            if (auto scene = m_activeScene.lock(); scene)
                scene->addEntity("Entity" + std::to_string(m_entityIndex++));

        using sl::scene::components::TransformComponent;
        auto scene = m_activeScene.lock();
        if (scene) {
            gui.breakLine();
            for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
                auto onEntityClick = [&]() {
                    m_selectedEntity = entity;

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
        }
        gui.endPanel();

        if (scene) {
            if (m_selectedEntity != nullptr && m_selectedEntity->hasComponent<TransformComponent>()) {
                auto& transform = m_selectedEntity->getComponent<TransformComponent>();

                gui.setupGizmo(scene->camera->viewFrustum.viewport);

                auto viewMatrix = scene->camera->getViewMatrix();
                auto projectionMatrix = scene->camera->getProjectionMatrix();

                auto transformationMatrix = transform.transformation;

                gui.manipulateGizmo(viewMatrix, projectionMatrix, transformationMatrix,
                    sl::gui::GizmoOperation::translate, sl::gui::GizmoSystem::world);

                if (gui.isUsingGizmo()) {
                    math::Vec3 rotation;
                    math::decomposeMatrix(transformationMatrix, transform.position, rotation, transform.scale);

                    transform.rotation += rotation;
                    transform.recalculate();
                }
            }
        }
    }

private:
    WidgetPosition m_position;

    std::shared_ptr<sl::ecs::Entity>& m_selectedEntity;
    int m_entityIndex;
};
}
