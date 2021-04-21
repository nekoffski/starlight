#include "EntityTab.h"

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

EntityTab::EntityTab(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState) {
}

void EntityTab::render(sl::gui::GuiApi& gui) {
    auto& widgetProperties = m_sharedState->guiProperties.propertiesPanelProperties;
    gui.beginTabBar("lowerLeftTabBar");

    if (gui.beginTabItem(ICON_FA_CUBE " Entity")) {
        showEntityProperties(gui);
        gui.endTabItem();
    }

    gui.endTabBar();
}

void EntityTab::showEntityProperties(sl::gui::GuiApi& gui) {
    if (auto selectedEntity = m_sharedState->selectedEntity.lock(); selectedEntity) {
        static int selectedValue = 0;

        auto& widgetProperties = m_sharedState->guiProperties.rightPanelProperties;
        if (gui.button(ICON_FA_PLUS " Add component", gui.getCurrentWindowWidth())) {
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
                addModel(load, *selectedEntity, gui);
                break;
            }

            case 1: {
                addRenderer(load, *selectedEntity, gui);
                break;
            }

            case 2: {
                addRenderer(load, *selectedEntity, gui);
                break;
            }

            case 3: {
                addParticleEffect(load, *selectedEntity, gui);
                break;
            }

            case 4: {
                addTransform(load, *selectedEntity, gui);
                break;
            }

            case 5: {
                addPointLight(load, *selectedEntity, gui);
                break;
            }

            case 6: {
                addDirectionalLight(load, *selectedEntity, gui);
                break;
            }
            }
            gui.endGroup();
            gui.endPopUp();
        }

        selectedEntity->onGui(gui);
    }
}

void EntityTab::addModel(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui) {
    auto modelsNames = m_sharedState->assetManager.getNamesByType(sl::asset::AssetType::model);
    modelsNames.insert(modelsNames.begin(), "None");

    static int selectedValue = 0;
    gui.displayText("Model");
    gui.sameLine();
    gui.combo(sl::gui::createHiddenLabel("Model"), selectedValue, modelsNames);

    if (load && selectedValue != 0) {
        auto& modelName = modelsNames[selectedValue];
        auto modelAsset =
            m_sharedState->assetManager.getAssetsByType(sl::asset::AssetType::model)[modelName]->as<sl::asset::ModelAsset>();
        entity.addComponent<sl::scene::components::ModelComponent>(modelAsset->model);
    }
}

void EntityTab::addParticleEffect(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui) {
    if (load)
        entity.addComponent<scene::components::ParticleEffectComponent>();
}

void EntityTab::addPointLight(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui) {
    if (load)
        entity.addComponent<sl::scene::components::PointLightComponent>();
}

void EntityTab::addDirectionalLight(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui) {
    if (load)
        entity.addComponent<sl::scene::components::DirectionalLightComponent>();
}

void EntityTab::addRenderer(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui) {
    auto shadersNames = m_sharedState->assetManager.getNamesByType(sl::asset::AssetType::shader);
    shadersNames.insert(shadersNames.begin(), "None");

    static int selectedValue = 0;
    gui.displayText("Shader");
    gui.sameLine();
    gui.combo(sl::gui::createHiddenLabel("Shader"), selectedValue, shadersNames);

    if (load && selectedValue != 0) {
        auto& shaderName = shadersNames[selectedValue];
        auto shaderAsset =
            m_sharedState->assetManager.getAssetsByType(sl::asset::AssetType::shader)[shaderName]->as<sl::asset::ShaderAsset>();
        entity.addComponent<sl::scene::components::RendererComponent>(shaderAsset->shader);
    }
}

void EntityTab::addTransform(bool load, sl::ecs::Entity& entity, sl::gui::GuiApi& gui) {
    if (load) {
        entity.addComponent<sl::scene::components::TransformComponent>();
    }
}
}
