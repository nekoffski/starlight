#pragma once

#include <memory>

#include "sl/platform/texture/Cubemap.h"
#include "sl/core/event/EventBus.h"
#include "sl/gui/GUIProxy.h"
#include "sl/gui/Utils.hpp"

#include "Widget.h"
#include "editor/res/ResourceManager.h"

namespace editor::gui {

using namespace sl::core;

class PropertiesPanel : public Widget {
public:
    explicit PropertiesPanel(const WidgetPosition& position, res::ResourceManager& resourceManager)
        : m_position(position)
        , m_resourceManager(resourceManager) {
    }

    void render(sl::gui::GUIProxy& gui) override {
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
                    SL_INFO("NAME {}", cubemapName);
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
            gui->endTabItem();
        }

        gui->endTabBar();
        gui->endPanel();
    }

private:
    WidgetPosition m_position;
    res::ResourceManager& m_resourceManager;
    std::weak_ptr<sl::platform::texture::Cubemap> m_selectedCubemap;
};
}
