#pragma once

#include <memory>
#include <string>

#include "sl/ecs/Entity.h"
#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"
#include "sl/scene/components/TransformComponent.h"

#include "sl/core/Logger.h"

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
        if (gui.button(ICON_FA_PLUS "  Add entity", gui.getCurrentWindowWidth()))
            if (scene)
                scene->addEntity("Entity" + std::to_string(scene->getEntitiesCount()));

        using sl::scene::components::TransformComponent;

        gui.pushId("scene-panel");

        if (scene) {
            gui.breakLine();

            std::vector<std::string> entitiesToRemove;

            if (gui.beginTreeNode(" " ICON_FA_CUBES "  Scene")) {
                for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
                    gui.pushId(entityId);

                    auto onEntityClick = [&]() {
                        m_sharedState->selectedEntityId = entity->getId();

                        if (entity->hasComponent<TransformComponent>()) {
                            auto& transform = entity->getComponent<TransformComponent>();
                            event::Emitter::emit<event::ChangeSceneCenterEvent>(transform.position);
                        }
                    };

                    // auto selectedEntity = m_sharedState->selectedEntity.lock();

                    bool isEntitySelected = m_sharedState->selectedEntityId.has_value() &&
                        m_sharedState->selectedEntityId.value() == entityId;

                    auto entryColor =
                        isEntitySelected ? sl::gui::selectedEntryColor : entity->isActive ? sl::gui::guiDefaultTextColor
                                                                                          : sl::gui::disabledEntryColor;

                    gui.pushTextColor(entryColor);

                    bool isEntityOpened = gui.beginTreeNode(" " ICON_FA_CUBE "  "s + entity->getName(), false);
                    bool isClicked = gui.isPreviousWidgetClicked();

                    gui.popColor();

                    gui.sameLine();
                    gui.setFontScale(0.55f);

                    if (gui.checkbox("##isActive", entity->isActive)) {
                        SL_INFO("click");
                        for (auto& componentIndex : entity->getComponentsIndexes())
                            entity->getComponent(componentIndex).isActive = entity->isActive;
                    }

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

        gui.popId();
        gui.endPanel();

        if (scene) {
            if (m_sharedState->hasSelectedEntity()) {
                auto& selectedEntity = m_sharedState->getSelectedEntity();

                if (selectedEntity.hasComponent<TransformComponent>()) {
                    auto& transform = selectedEntity.getComponent<TransformComponent>();

                    gui.setupGizmo(scene->camera->viewFrustum.viewport);

                    auto viewMatrix = scene->camera->getViewMatrix();
                    auto projectionMatrix = scene->camera->getProjectionMatrix();

                    auto transformationMatrix = transform.transformation;

                    gui.manipulateGizmo(viewMatrix, projectionMatrix, transformationMatrix,
                        m_sharedState->gizmoOperation, m_sharedState->gizmoSystem);

                    if (gui.isUsingGizmo()) {
                        math::Vec3 rotation;
                        math::decomposeMatrix(transformationMatrix, transform.position,
                            rotation, transform.scale);

                        transform.rotation = sl::math::toDegrees(rotation);
                        transform.recalculateTransformation();
                    }
                }
            }
        }
    }

private:
    int m_entityIndex;
};
}
