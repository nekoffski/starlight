#include "PropertiesPanel.h"

#include "sl/core/event/EventBus.h"
#include "sl/gui/Utils.hpp"
#include "sl/scene/components/ModelComponent.h"

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
        showSceneProperties(gui);
        gui->endTabItem();
    }

    if (gui->beginTabItem("Entity")) {
        showEntityProperties(gui);
        gui->endTabItem();
    }

    gui->endTabBar();
    gui->endPanel();
}

void PropertiesPanel::showSceneProperties(sl::gui::GuiProxy& gui) {
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
}

void PropertiesPanel::showEntityProperties(sl::gui::GuiProxy& gui) {
    if (m_selectedEntity != nullptr) {
        static int selectedValue = 0;

        if (gui->button("Add component", m_position.size.x)) {
            selectedValue = 0;
            gui->openPopUp("AddComponentPopUp");
        }

        if (gui->beginPopUp("AddComponentPopUp")) {
            static std::vector<std::string> componentsNames = {
                "Model", "Renderer", "RigidBody", "ParticleEffect", "Transform"
            };

            gui->beginGroup();
            gui->combo(sl::gui::createHiddenLabel("ComponentCombo"), selectedValue, componentsNames);

            bool load = false;
            if (gui->button("Add")) {
                gui->closeCurrentPopUp();
                load = true;
            }

            gui->endGroup();
            gui->sameLine(250);
            gui->beginGroup();

            switch (selectedValue) {
            case 0: {
                addModel(load, gui);
                break;
            }

            case 1: {
                addRenderer(load, gui);
                break;
            }

            case 2:
                break;
            case 3:
                break;

            case 4: {
                addTransform(load, gui);
                break;
            }
            }
            gui->endGroup();
        }

        m_selectedEntity->onGui(gui);
    }
}

void PropertiesPanel::addModel(bool load, sl::gui::GuiProxy& gui) {
    auto modelsNames = m_resourceManager.getNamesByType(res::ResourceType::MODEL);
    modelsNames.insert(modelsNames.begin(), "None");

    int selectedValue = 0;
    gui->displayText("Model");
    gui->sameLine();
    gui->combo(sl::gui::createHiddenLabel("Model"), selectedValue, modelsNames);

    if (load && selectedValue != 0) {
        auto& modelName = modelsNames[selectedValue];
        auto modelResource =
            m_resourceManager.getResourcesByType(res::ResourceType::MODEL)[modelName]->as<res::ModelResource>();
        m_selectedEntity->addComponent<sl::scene::components::ModelComponent>(modelResource->model);
    }
}

void PropertiesPanel::addRenderer(bool load, sl::gui::GuiProxy& gui) {
}

void PropertiesPanel::addTransform(bool load, sl::gui::GuiProxy& gui) {
}
}
