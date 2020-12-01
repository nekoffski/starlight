#include "BottomPanel.h"

#include "sl/core/event/EventBus.h"

namespace editor::gui {

BottomPanel::BottomPanel(const Settings& settings, res::ResourceManager& resourceManager)
    : m_position(settings.bottomPanelPosition)
    , m_resourcesTab(resourceManager) {
}

void BottomPanel::render(sl::gui::GUIProxy& gui) {
    gui->beginPanel(sl::gui::createHiddenLabel("BottomMenu"), m_position.origin, m_position.size);

    gui->beginTabBar("bottomTabBar");

    if (gui->beginTabItem("Resources")) {
        m_resourcesTab.render(gui);
        gui->endTabItem();
    }

    if (gui->beginTabItem("Debug console")) {
        gui->endTabItem();
    }

    gui->endTabBar();
    gui->endPanel();
}

}
