#include "BottomPanel.h"

#include "sl/gui/Utils.hpp"

namespace editor::gui {

BottomPanel::BottomPanel(const Settings& settings, sl::asset::AssetManager& assetManager)
    : m_position(settings.bottomPanelPosition)
    , m_assetsTab(assetManager) {
}

void BottomPanel::render(sl::gui::GuiApi& gui) {
    gui.beginPanel(sl::gui::createHiddenLabel("BottomMenu"), m_position.origin, m_position.size);

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
