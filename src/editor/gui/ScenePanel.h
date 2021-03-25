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

#include "Widget.h"

namespace editor::gui {

using namespace sl;

class ScenePanel : public Widget {
public:
    explicit ScenePanel(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState)
        , m_entityIndex(0) {
    }

    void render(sl::gui::GuiApi& gui) override {
        auto& widgetProperties = m_sharedState->guiProperties.scenePanelProperties;

        auto scene = m_sharedState->activeScene.lock();

        gui.beginPanel("Scene entities", widgetProperties.origin, widgetProperties.size);
        if (gui.button(ICON_FA_PLUS " Add entity", 150))
            if (scene)
                scene->addEntity("Entity" + std::to_string(scene->getEntitiesCount()));

        using sl::scene::components::TransformComponent;

        if (scene) {
            gui.breakLine();

            if (gui.beginTreeNode(" " ICON_FA_CUBES " Scene")) {
                for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
                    auto onEntityClick = [&]() {
                        m_sharedState->selectedEntity = entity;

                        if (entity->hasComponent<TransformComponent>()) {
                            auto& transform = entity->getComponent<TransformComponent>();
                            event::Emitter::emit<event::ChangeSceneCenterEvent>(transform.position);
                        }
                    };

                    bool isSelectedEntity = m_sharedState->selectedEntity != nullptr &&
                        m_sharedState->selectedEntity->getId() == entityId;

                    if (isSelectedEntity)
                        gui.pushTextColor(selectedEntryColor);

                    if (gui.beginTreeNode(" " ICON_FA_CUBE " "s + entity->getName(), false)) {
                        gui.pushTextColor(guiDefaultTextColor);
                        if (gui.isPreviousWidgetClicked())
                            onEntityClick();

                        gui.popColor();
                        gui.popTreeNode();
                    }

                    if (isSelectedEntity)
                        gui.popColor();

                    if (gui.isPreviousWidgetClicked())
                        onEntityClick();
                }
            }
        }
        gui.endPanel();

        if (scene) {
            if (m_sharedState->selectedEntity != nullptr && m_sharedState->selectedEntity->hasComponent<TransformComponent>()) {
                auto& transform = m_sharedState->selectedEntity->getComponent<TransformComponent>();

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
    int m_entityIndex;
};
}
