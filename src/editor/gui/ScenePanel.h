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

            std::vector<std::string> entitiesToRemove;

            if (gui.beginTreeNode(" " ICON_FA_CUBES " Scene")) {
                for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
                    gui.pushId(entity->getId());

                    auto onEntityClick = [&]() {
                        m_sharedState->selectedEntity = entity;

                        if (entity->hasComponent<TransformComponent>()) {
                            auto& transform = entity->getComponent<TransformComponent>();
                            event::Emitter::emit<event::ChangeSceneCenterEvent>(transform.position);
                        }
                    };

                    auto selectedEntity = m_sharedState->selectedEntity.lock();
                    bool isEntitySelected = selectedEntity && selectedEntity->getId() == entityId;

                    auto entryColor =
                        isEntitySelected ? selectedEntryColor : entity->isActive ? guiDefaultTextColor
                                                                                 : disabledEntryColor;

                    gui.pushTextColor(entryColor);

                    bool isEntityOpened = gui.beginTreeNode(" " ICON_FA_CUBE " "s + entity->getName(), false);
                    bool isClicked = gui.isPreviousWidgetClicked();

                    gui.popColor();

                    gui.sameLine();
                    gui.setFontScale(0.55f);
                    gui.checkbox("##isActive", entity->isActive);
                    gui.setFontScale(0.8);
                    gui.sameLine();
                    gui.displayText(ICON_FA_TIMES);

                    if (gui.isPreviousWidgetClicked())
                        entitiesToRemove.push_back(entityId);

                    gui.setFontScale(1.0f);

                    if (isEntityOpened) {
                        if (isClicked)
                            onEntityClick();

                        gui.popTreeNode();
                    }

                    if (isClicked)
                        onEntityClick();

                    gui.popId();
                }

                for (auto& entityId : entitiesToRemove)
                    scene->ecsRegistry.removeEntity(entityId);
            }
        }
        gui.endPanel();

        if (scene) {
            if (auto selectedEntity = m_sharedState->selectedEntity.lock(); selectedEntity &&
                selectedEntity->hasComponent<TransformComponent>()) {

                auto& transform = selectedEntity->getComponent<TransformComponent>();

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
