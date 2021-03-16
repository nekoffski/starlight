#include "BottomPanel.h"

#include "sl/gui/Utils.hpp"

namespace editor::gui {

BottomPanel::BottomPanel(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState)
    , m_assetsTab(sharedState)
    , m_debugConsoleTab(sharedState) {
}

void BottomPanel::render(sl::gui::GuiApi& gui) {
    auto& widgetProperties = m_sharedState->guiProperties.bottomPanelProperties;
    gui.beginPanel(sl::gui::createHiddenLabel("BottomMenu"), widgetProperties.origin, widgetProperties.size);

    gui.beginTabBar("bottomTabBar");

    if (gui.beginTabItem("Assets")) {
        m_assetsTab.render(gui);
        gui.endTabItem();
    }

    if (gui.beginTabItem("Debug console")) {
        m_debugConsoleTab.render(gui);
        gui.endTabItem();
    }

    gui.endTabBar();
    gui.endPanel();
}
}
