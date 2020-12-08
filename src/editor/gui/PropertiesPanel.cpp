#include "PropertiesPanel.h"

#include "sl/core/event/EventBus.h"
#include "sl/gui/Utils.hpp"

namespace editor::gui {

using namespace sl::core;

PropertiesPanel::PropertiesPanel(const WidgetPosition& position, res::ResourceManager& resourceManager,
    std::shared_ptr<sl::ecs::Entity>& selectedEntity)
    : m_position(position)
    , m_resourceManager(resourceManager)
    , m_selectedEntity(selectedEntity) {
}

void PropertiesPanel::render(sl::gui::GuiProxy& gui) {
    gui->beginPanel("Properties", m_position.origin, m_position.size);

    gui->beginTabBar("lowerLeftTabBar");

    if (gui->beginTabItem("Scene")) {
        if (gui->beginTreeNode("Skybox")) {
            auto cubemapsNames = m_resourceManager.getNamesByType(res::ResourceType::CUBEMAP);
            cubemapsNames.insert(cubemapsNames.begin(), "None");

            int selectedValue = 0;
            gui->displayText("Cubemap");
            gui->sameLine();
            gui->combo(sl::gui::createHiddenLabel("Cubemap"), selectedValue, cubemapsNames);

            if (selectedValue != 0) {
                auto& cubemapName = cubemapsNames[selectedValue];
                auto cubemap =
                    m_resourceManager.getResourcesByType(res::ResourceType::CUBEMAP)[cubemapName]->as<res::CubemapResource>()->cubemap;

                m_selectedCubemap = cubemap;
                event::EventBus::emitEvent<event::SetSkyboxEvent>(cubemap);
            }
            gui->popTreeNode();
        }
        gui->endTabItem();
    }

    if (gui->beginTabItem("Entity")) {
		if (m_selectedEntity != nullptr) {
			gui->displayText(m_selectedEntity->getName());
			m_selectedEntity->onGui(gui);
		}
		gui->endTabItem();
    }

    gui->endTabBar();
    gui->endPanel();
}
}
