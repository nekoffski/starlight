#include "EntityTab.h"

#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"
#include "sl/gui/Utils.hpp"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

#include "sl/physx/AxisAlignedBoundingBox.h"

namespace editor::gui {

using namespace sl;

EntityTab::EntityTab(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState)
    , m_selectedComponent(0) {
}

void EntityTab::render(sl::gui::GuiApi& gui) {
    auto& widgetProperties = m_sharedState->guiProperties.propertiesPanelProperties;
    if (gui.beginTabItem(ICON_FA_CUBE " Entity")) {
        showEntityProperties(gui);
        gui.endTabItem();
    }
}

// clang-format off
template <typename T, int expectedIndex>
requires std::derived_from<T, ecs::Component> 
static void handleComponent(int index, ecs::Entity& entity) {
    if (index == expectedIndex) {
        if (entity.hasComponent<T>()) {
            auto& componentName = entity.getComponent<T>().name;
            SL_WARN("Could not add {} to {}", componentName, entity.asString());

            throw core::GuiUserError{ core::ErrorCode::GuiUserError, 
                "Entity already has " + componentName };
        }
        
        auto& component = entity.addComponent<T>();
        SL_INFO("Added {} to {}", component.name, entity.asString());
    }
}
// clang-format on

void EntityTab::showEntityProperties(sl::gui::GuiApi& gui) {
    if (auto selectedEntity = m_sharedState->selectedEntity.lock(); selectedEntity) {

        auto& widgetProperties = m_sharedState->guiProperties.rightPanelProperties;
        if (gui.button(ICON_FA_PLUS " Add component", gui.getCurrentWindowWidth())) {
            m_selectedComponent = 0;
            gui.openPopUp("AddComponentPopUp");
        }

        if (gui.beginPopUp("AddComponentPopUp")) {
            static std::vector<std::string> componentsNames = {
                "Model", "Renderer", "Rigid body", "Particle effect", "Transform", "Point light", "Directional light", "Material"
            };

            gui.beginGroup();
            gui.combo(sl::gui::createHiddenLabel("ComponentCombo"), m_selectedComponent, componentsNames);

            if (gui.button("Add")) {
                using namespace scene::components;
                gui.closeCurrentPopUp();

                try {
                    // clang-format off
                    handleComponent<ModelComponent,            0>(m_selectedComponent, *selectedEntity);
                    handleComponent<MeshRendererComponent,     1>(m_selectedComponent, *selectedEntity);
                    handleComponent<RigidBodyComponent,        2>(m_selectedComponent, *selectedEntity);
                    handleComponent<ParticleEffectComponent,   3>(m_selectedComponent, *selectedEntity);
                    handleComponent<TransformComponent,        4>(m_selectedComponent, *selectedEntity);
                    handleComponent<PointLightComponent,       5>(m_selectedComponent, *selectedEntity);
                    handleComponent<DirectionalLightComponent, 6>(m_selectedComponent, *selectedEntity);
                    handleComponent<MaterialComponent,         7>(m_selectedComponent, *selectedEntity);
                    // clang-format on

                } catch (core::GuiUserError& e) {
                    m_errorDialog.setErrorMessage(e.getDetails());
                }
            }

            gui.endGroup();
            gui.endPopUp();
        }

        m_errorDialog.show(gui);

        m_entityGui.renderEntityGui(*selectedEntity, gui, m_sharedState->assetManager);
    }
}
}
