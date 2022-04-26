#include "PropertiesPanel.h"

#include <imgui_sugar.hpp>
#include <kc/core/Utils.hpp>

#include "sl/event/Event.h"
#include "sl/event/EventManager.h"
#include "sl/glob/Globals.h"
#include "sl/gui/Utils.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/ParticleEffectComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace editor::gui {

using namespace sl;

PropertiesPanel::PropertiesPanel(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState), m_assetTab(sharedState), m_systemTab(sharedState) {}

void PropertiesPanel::render() {
    auto& widgetProperties = m_sharedState->guiProperties.propertiesPanelProperties;

    sl::gui::beginPanel("Properties", widgetProperties.origin, widgetProperties.size);
    with_TabBar("lowerLeftTabBar") {
        with_TabItem(ICON_FA_GLOBE_EUROPE "  Scene") showSceneProperties();

        m_assetTab.render();
        m_systemTab.render();
    }
    sl::gui::endPanel();
}

void PropertiesPanel::showSceneProperties() {
    with_OpenedTreeNode(ICON_FA_CLOUD " Skybox") {
        auto cubemapsNames = m_sharedState->assetManager.getCubemaps().getNames();
        cubemapsNames.insert(cubemapsNames.begin(), "None");

        static int selectedValue         = 0;
        static int previousSelectedValue = -1;

        ImGui::Text("Cubemap");
        ImGui::SameLine();
        sl::gui::combo("##Cubemap", &selectedValue, cubemapsNames);

        if (auto scene = m_sharedState->activeScene; scene) {
            // if (selectedValue == 0 && scene->skybox != nullptr)
            //     scene->skybox = nullptr;

            if (scene->skybox)
                selectedValue = kc::core::indexOf(cubemapsNames, scene->skybox->cubemap->name, 0);
        }

        if (selectedValue != 0 && selectedValue != previousSelectedValue) {
            auto& cubemapName = cubemapsNames[selectedValue];
            auto cubemap      = m_sharedState->assetManager.getCubemaps().getByName(cubemapName);

            m_selectedCubemap = cubemap;
            event::EventManager::get().emit<event::SetSkyboxEvent>(cubemap).toAll();

            previousSelectedValue = selectedValue;
        }
    }

    with_OpenedTreeNode(ICON_FA_VIDEO "  Camera") {
        if (auto scene = m_sharedState->activeScene; scene) scene->camera->onGui();
    }

    ImGui::Text("\n");

    with_OpenedTreeNode(ICON_FA_COGS "  Properties") {
        ImGui::Text("Gravity acceleration");
        ImGui::SliderFloat2(
            "##Gravity acceleration", &glob::Globals::get().world.gravity.y, 0.0f, 25.0f
        );
    }

    ImGui::Text("\n");

    with_OpenedTreeNode(ICON_FA_EYE "  Visual") {
        ImGui::Text("Gamma corection");
        ImGui::SliderFloat(
            "##Gamma correction", &glob::Globals::get().visual.gammaCorrection, 0.0f, 5.0f
        );
        ImGui::Text("Exposure");
        ImGui::SliderFloat("##Exposure", &glob::Globals::get().visual.exposure, 0.0f, 5.0f);
        ImGui::Checkbox("Enable bloom", &glob::Globals::get().visual.enableBloom);
    }
}
}  // namespace editor::gui
