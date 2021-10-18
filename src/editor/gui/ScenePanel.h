#pragma once

#include <memory>
#include <string>

#include "sl/ecs/Entity.h"
#include "sl/event/Event.h"
#include "sl/event/EventManager.h"
#include "sl/math/Utils.hpp"
#include "sl/math/Vector.hpp"
#include "sl/scene/components/TransformComponent.h"

#include <kc/core/Log.h>

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

    void render() override {
        auto& widgetProperties = m_sharedState->guiProperties.scenePanelProperties;

        auto scene = m_sharedState->activeScene.lock();

        sl::gui::beginPanel("Scene entities", widgetProperties.origin, widgetProperties.size);
        if (ImGui::Button(ICON_FA_PLUS "  Add entity", ImVec2(ImGui::GetWindowWidth(), 0)))
            if (scene)
                scene->addEntity("Entity" + std::to_string(scene->getEntitiesCount()));

        using sl::scene::components::TransformComponent;

        with_ID("scene-panel") {

            if (scene) {
                ImGui::Separator();

                std::vector<std::string> entitiesToRemove;

                with_OpenedTreeNode(" " ICON_FA_CUBES "  Scene") {
                    for (auto& [entityId, entity] : scene->ecsRegistry.getEntities()) {
                        with_ID(entityId.c_str()) {

                            auto onEntityClick = [&]() {
                                m_sharedState->selectedEntityId = entity->getId();

                                if (entity->hasComponent<TransformComponent>()) {
                                    auto& transform = entity->getComponent<TransformComponent>();
                                    event::EventManager::get()->emit<event::ChangeSceneCenterEvent>(transform.position).toAll();
                                }
                            };

                            bool isEntitySelected = m_sharedState->selectedEntityId.has_value() &&
                                m_sharedState->selectedEntityId.value() == entityId;

                            auto entryColor =
                                isEntitySelected ? sl::gui::selectedEntryColor : entity->isActive ? sl::gui::guiDefaultTextColor
                                                                                                  : sl::gui::disabledEntryColor;

                            sl::gui::pushTextColor(entryColor);

                            static std::string gap = "  ";

                            ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
                            bool isEntityOpened = ImGui::TreeNode((gap + ICON_FA_CUBE + gap + entity->getName()).c_str());
                            bool isClicked = ImGui::IsItemClicked();

                            sl::gui::popTextColor();

                            ImGui::SameLine();
                            ImGui::SetWindowFontScale(0.55f);

                            if (ImGui::Checkbox("##isActive", &entity->isActive)) {
                                LOG_INFO("click");
                                for (auto& componentIndex : entity->getComponentsIndexes())
                                    entity->getComponent(componentIndex).isActive = entity->isActive;
                            }

                            ImGui::SetWindowFontScale(0.8);
                            ImGui::SameLine();
                            ImGui::Text(ICON_FA_TIMES);

                            if (ImGui::IsItemClicked())
                                entitiesToRemove.push_back(entityId);

                            ImGui::SetWindowFontScale(1.0f);

                            if (isEntityOpened) {
                                if (isClicked)
                                    onEntityClick();

                                ImGui::TreePop();
                            }

                            if (isClicked)
                                onEntityClick();
                        }
                    }

                    for (auto& entityId : entitiesToRemove)
                        scene->ecsRegistry.removeEntity(entityId);
                }
            }
        }

        sl::gui::endPanel();

        if (scene) {
            if (m_sharedState->hasSelectedEntity()) {
                auto& selectedEntity = m_sharedState->getSelectedEntity();

                if (selectedEntity.hasComponent<TransformComponent>()) {
                    auto& transform = selectedEntity.getComponent<TransformComponent>();

                    sl::gui::setupGizmo(scene->camera->viewFrustum.viewport);

                    auto viewMatrix = scene->camera->getViewMatrix();
                    auto projectionMatrix = scene->camera->getProjectionMatrix();

                    auto transformationMatrix = transform.transformation;

                    sl::gui::manipulateGizmo(viewMatrix, projectionMatrix, transformationMatrix,
                        m_sharedState->gizmoOperation, m_sharedState->gizmoSpace);

                    if (sl::gui::isUsingGizmo()) {
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
