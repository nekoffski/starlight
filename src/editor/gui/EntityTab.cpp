#include "EntityTab.h"

#include "sl/event/Event.h"
#include "sl/gui/Utils.h"
#include "sl/physx/AxisAlignedBoundingBox.h"
#include "sl/scene/components/CameraComponent.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace editor::gui {

using namespace sl;

EntityTab::EntityTab(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState), m_selectedComponent(0) {}

void EntityTab::render() {
    auto& widgetProperties = m_sharedState->guiProperties.propertiesPanelProperties;
    with_TabItem(ICON_FA_CUBE " Entity") showEntityProperties();
}

// clang-format off
template <typename T, int expectedIndex>
requires std::derived_from<T, ecs::Component> 
static void handleComponent(int index, ecs::Entity& entity) {
    if (index == expectedIndex) {
        if (entity.hasComponent<T>()) {
            auto& componentName = entity.getComponent<T>().name;
            LOG_WARN("Could not add {} to {}", componentName, entity.asString());

            throw core::GuiUserError{ "Entity already has " + componentName };
        }
        
        auto& component = entity.addComponent<T>();
        LOG_INFO("Added {} to {}", component.name, entity.asString());
    }
}
// clang-format on

void EntityTab::showEntityProperties() {
    if (m_sharedState->hasSelectedEntity()) {
        auto& selectedEntity = m_sharedState->getSelectedEntity();

        auto entityId = selectedEntity.getId();

        if (not m_entityNamePlacehoders.contains(entityId))
            m_entityNamePlacehoders[entityId] = selectedEntity.getName();

        auto& namePlaceholder = m_entityNamePlacehoders.at(entityId);

        ImGui::Text("Entity name:");
        sl::gui::textInput("##entity_name", namePlaceholder);
        ImGui::SameLine();

        if (ImGui::Button(ICON_FA_CHECK_CIRCLE)) {
            auto& ecs = m_sharedState->activeScene->ecsRegistry;

            if (ecs.hasEntityByName(namePlaceholder))
                m_errorDialog.setErrorMessage(
                    fmt::format("Entity with name {} already exists", namePlaceholder)
                );
            else
                selectedEntity.setName(namePlaceholder);
        }

        ImGui::Text("\n");
        ImGui::Separator();

        auto& widgetProperties = m_sharedState->guiProperties.rightPanelProperties;
        if (ImGui::Button(ICON_FA_PLUS " Add component", ImVec2(ImGui::GetWindowWidth(), 0))) {
            m_selectedComponent = 0;
            ImGui::OpenPopup("AddComponentPopUp");
        }

        with_Popup("AddComponentPopUp") {
            static std::vector<std::string> componentsNames = {
                "Model",       "Renderer",          "Rigid body", "Particle effect", "Transform",
                "Point light", "Directional light", "Material",   "Camera"};

            with_Group {
                sl::gui::combo("##ComponentCombo", &m_selectedComponent, componentsNames);

                if (ImGui::Button("Add")) {
                    using namespace scene::components;
                    ImGui::CloseCurrentPopup();

                    try {
                        // clang-format off
                        handleComponent<ModelComponent,            0>(m_selectedComponent, selectedEntity);
                        handleComponent<MeshRendererComponent,     1>(m_selectedComponent, selectedEntity);
                        handleComponent<RigidBodyComponent,        2>(m_selectedComponent, selectedEntity);
                        handleComponent<ParticleEffectComponent,   3>(m_selectedComponent, selectedEntity);
                        handleComponent<TransformComponent,        4>(m_selectedComponent, selectedEntity);
                        handleComponent<PointLightComponent,       5>(m_selectedComponent, selectedEntity);
                        handleComponent<DirectionalLightComponent, 6>(m_selectedComponent, selectedEntity);
                        handleComponent<MaterialComponent,         7>(m_selectedComponent, selectedEntity);
                        handleComponent<CameraComponent,           8>(m_selectedComponent, selectedEntity);
                        // clang-format on

                    } catch (core::GuiUserError& e) {
                        m_errorDialog.setErrorMessage(e.getDetails());
                    }
                }
            }
        }
        m_errorDialog.show();
        m_entityGui.renderEntityGui(selectedEntity, m_sharedState->assetManager);
    }
}
}  // namespace editor::gui
