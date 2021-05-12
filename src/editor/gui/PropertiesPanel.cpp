#include "PropertiesPanel.h"

#include "sl/core/Utils.hpp"
#include "sl/event/Emitter.hpp"
#include "sl/event/Event.h"
#include "sl/gui/Utils.hpp"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace editor::gui {

using namespace sl;

PropertiesPanel::PropertiesPanel(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState) {
}

void PropertiesPanel::render(sl::gui::GuiApi& gui) {
    auto& widgetProperties = m_sharedState->guiProperties.propertiesPanelProperties;

    gui.beginPanel("Properties", widgetProperties.origin, widgetProperties.size);
    gui.beginTabBar("lowerLeftTabBar");

    if (gui.beginTabItem(ICON_FA_GLOBE_EUROPE "  Scene")) {
        showSceneProperties(gui);
        gui.endTabItem();
    }

    gui.endTabBar();
    gui.endPanel();
}

void PropertiesPanel::showSceneProperties(sl::gui::GuiApi& gui) {
    if (gui.beginTreeNode(ICON_FA_CLOUD " Skybox")) {
        auto cubemapsNames = m_sharedState->assetManager.getCubemaps().getNames();
        cubemapsNames.insert(cubemapsNames.begin(), "None");

        static int selectedValue = 0;
        gui.displayText("Cubemap");
        gui.sameLine();
        gui.combo(sl::gui::createHiddenLabel("Cubemap"), selectedValue, cubemapsNames);

        if (auto scene = m_sharedState->activeScene.lock(); scene)
            if (scene->skybox != nullptr)
                selectedValue = core::indexOf(cubemapsNames, scene->skybox->cubemap->name, 0);

        if (selectedValue != 0) {
            auto& cubemapName = cubemapsNames[selectedValue];
            auto cubemap = m_sharedState->assetManager.getCubemaps().getByName(cubemapName);

            m_selectedCubemap = cubemap;
            event::Emitter::emit<event::SetSkyboxEvent>(cubemap);
        }
        gui.popTreeNode();
    }

    if (gui.beginTreeNode(ICON_FA_VIDEO "  Camera")) {
        if (auto scene = m_sharedState->activeScene.lock(); scene)
            scene->camera->onGui(gui);
        gui.popTreeNode();
    }

    gui.displayText("\n");

    if (gui.beginTreeNode(ICON_FA_COGS "  Properties")) {
        gui.displayText("Gravity acceleration");
        gui.sliderFloat("##Gravity acceleration", GLOBALS().world.gravity.y, 0.0f, 25.0f);
        gui.popTreeNode();
    }
}
}
