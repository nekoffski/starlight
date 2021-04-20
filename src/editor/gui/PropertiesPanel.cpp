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

PropertiesPanel::PropertiesPanel(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState) {
}

void PropertiesPanel::render(sl::gui::GuiApi& gui) {
    auto& widgetProperties = m_sharedState->guiProperties.propertiesPanelProperties;

    gui.beginPanel("Properties", widgetProperties.origin, widgetProperties.size);
    gui.beginTabBar("lowerLeftTabBar");

    if (gui.beginTabItem("Scene")) {
        showSceneProperties(gui);
        gui.endTabItem();
    }

    gui.endTabBar();
    gui.endPanel();
}

void PropertiesPanel::showSceneProperties(sl::gui::GuiApi& gui) {
    if (gui.beginTreeNode("Skybox")) {
        auto cubemapsNames = m_sharedState->assetManager.getNamesByType(sl::asset::AssetType::cubemap);
        cubemapsNames.insert(cubemapsNames.begin(), "None");

        static int selectedValue = 0;
        gui.displayText("Cubemap");
        gui.sameLine();
        gui.combo(sl::gui::createHiddenLabel("Cubemap"), selectedValue, cubemapsNames);

        if (selectedValue != 0) {
            auto& cubemapName = cubemapsNames[selectedValue];
            auto cubemap =
                m_sharedState->assetManager.getAssetsByType(sl::asset::AssetType::cubemap)[cubemapName]->as<sl::asset::CubemapAsset>()->cubemap;

            m_selectedCubemap = cubemap;
            event::Emitter::emit<event::SetSkyboxEvent>(cubemap);
        }
        gui.popTreeNode();
    }

    if (gui.beginTreeNode("Camera")) {
        if (auto scene = m_sharedState->activeScene.lock(); scene)
            scene->camera->onGui(gui);
        gui.popTreeNode();
    }
}
}
