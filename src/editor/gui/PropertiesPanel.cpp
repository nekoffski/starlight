#include "PropertiesPanel.h"

#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"
#include "sl/gui/Utils.hpp"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/RendererComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace editor::gui {

using namespace sl;

PropertiesPanel::PropertiesPanel(const WidgetPosition& position, sl::asset::AssetManager& assetManager,
    std::shared_ptr<sl::ecs::Entity>& selectedEntity)
    : m_position(position)
    , m_assetManager(assetManager)
    , m_selectedEntity(selectedEntity) {
}

void PropertiesPanel::render(sl::gui::GuiApi& gui) {
    gui.beginPanel("Properties", m_position.origin, m_position.size);

    gui.beginTabBar("lowerLeftTabBar");

    if (gui.beginTabItem("Scene")) {
        showSceneProperties(gui);
        gui.endTabItem();
    }

    if (gui.beginTabItem("Entity")) {
        showEntityProperties(gui);
        gui.endTabItem();
    }

    gui.endTabBar();
    gui.endPanel();
}

void PropertiesPanel::showSceneProperties(sl::gui::GuiApi& gui) {
    if (gui.beginTreeNode("Skybox")) {
        auto cubemapsNames = m_assetManager.getNamesByType(sl::asset::AssetType::cubemap);
        cubemapsNames.insert(cubemapsNames.begin(), "None");

        int selectedValue = 0;
        gui.displayText("Cubemap");
        gui.sameLine();
        gui.combo(sl::gui::createHiddenLabel("Cubemap"), selectedValue, cubemapsNames);

        if (selectedValue != 0) {
            auto& cubemapName = cubemapsNames[selectedValue];
            auto cubemap =
                m_assetManager.getAssetsByType(sl::asset::AssetType::cubemap)[cubemapName]->as<sl::asset::CubemapAsset>()->cubemap;

            m_selectedCubemap = cubemap;
            event::Emitter::emit<event::SetSkyboxEvent>(cubemap);
        }
        gui.popTreeNode();
    }

    if (gui.beginTreeNode("Camera")) {
        if (auto scene = m_activeScene.lock(); scene)
            scene->camera->onGui(gui);
        gui.popTreeNode();
    }
}

void PropertiesPanel::showEntityProperties(sl::gui::GuiApi& gui) {
    if (m_selectedEntity != nullptr) {
        static int selectedValue = 0;

        if (gui.button("Add component", m_position.size.x)) {
            selectedValue = 0;
            gui.openPopUp("AddComponentPopUp");
        }

        if (gui.beginPopUp("AddComponentPopUp")) {
            static std::vector<std::string> componentsNames = {
                "Model", "Renderer", "Rigid body", "Particle effect", "Transform", "Point light", "Directional light"
            };

            gui.beginGroup();
            gui.combo(sl::gui::createHiddenLabel("ComponentCombo"), selectedValue, componentsNames);

            bool load = false;
            if (gui.button("Add")) {
                gui.closeCurrentPopUp();
                load = true;
            }

            gui.endGroup();
            gui.sameLine(250);
            gui.beginGroup();

            switch (selectedValue) {
            case 0: {
                addModel(load, gui);
                break;
            }

            case 1: {
                addRenderer(load, gui);
                break;
            }

            case 2: {
                addRenderer(load, gui);
                break;
            }

            case 3: {
                addParticleEffect(load, gui);
                break;
            }

            case 4: {
                addTransform(load, gui);
                break;
            }

            case 5: {
                addPointLight(load, gui);
                break;
            }

            case 6: {
                addDirectionalLight(load, gui);
                break;
            }
            }
            gui.endGroup();
            gui.endPopUp();
        }

        m_selectedEntity->onGui(gui);
    }
}

void PropertiesPanel::addModel(bool load, sl::gui::GuiApi& gui) {
    auto modelsNames = m_assetManager.getNamesByType(sl::asset::AssetType::model);
    modelsNames.insert(modelsNames.begin(), "None");

    static int selectedValue = 0;
    gui.displayText("Model");
    gui.sameLine();
    gui.combo(sl::gui::createHiddenLabel("Model"), selectedValue, modelsNames);

    if (load && selectedValue != 0) {
        auto& modelName = modelsNames[selectedValue];
        auto modelAsset =
            m_assetManager.getAssetsByType(sl::asset::AssetType::model)[modelName]->as<sl::asset::ModelAsset>();
        m_selectedEntity->addComponent<sl::scene::components::ModelComponent>(modelAsset->model);
    }
}

void PropertiesPanel::addParticleEffect(bool load, sl::gui::GuiApi& gui) {
    if (load)
        m_selectedEntity->addComponent<scene::components::ParticleEffectComponent>();
}

void PropertiesPanel::addPointLight(bool load, sl::gui::GuiApi& gui) {
    if (load)
        m_selectedEntity->addComponent<sl::scene::components::PointLightComponent>();
}

void PropertiesPanel::addDirectionalLight(bool load, sl::gui::GuiApi& gui) {
    if (load)
        m_selectedEntity->addComponent<sl::scene::components::DirectionalLightComponent>();
}

void PropertiesPanel::addRenderer(bool load, sl::gui::GuiApi& gui) {
    auto shadersNames = m_assetManager.getNamesByType(sl::asset::AssetType::shader);
    shadersNames.insert(shadersNames.begin(), "None");

    static int selectedValue = 0;
    gui.displayText("Shader");
    gui.sameLine();
    gui.combo(sl::gui::createHiddenLabel("Shader"), selectedValue, shadersNames);

    if (load && selectedValue != 0) {
        auto& shaderName = shadersNames[selectedValue];
        auto shaderAsset =
            m_assetManager.getAssetsByType(sl::asset::AssetType::shader)[shaderName]->as<sl::asset::ShaderAsset>();
        m_selectedEntity->addComponent<sl::scene::components::RendererComponent>(shaderAsset->shader);
    }
}

void PropertiesPanel::addTransform(bool load, sl::gui::GuiApi& gui) {
    if (load) {
        m_selectedEntity->addComponent<sl::scene::components::TransformComponent>();
    }
}
}
