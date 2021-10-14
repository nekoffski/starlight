#include "BottomPanel.h"

#include "sl/gui/Utils.hpp"

namespace editor::gui {

BottomPanel::BottomPanel(std::shared_ptr<SharedState> sharedState)
    : Widget(sharedState)
    , m_assetsTab(sharedState)
    , m_debugConsoleTab(sharedState) {
}

void BottomPanel::render() {
    auto& widgetProperties = m_sharedState->guiProperties.bottomPanelProperties;

    sl::gui::beginPanel("##BottomMenu", widgetProperties.origin, widgetProperties.size);

    with_TabBar("bottomTabBar") {

        with_TabItem("Assets")
            m_assetsTab.render();

        with_TabItem("Debug console")
            m_debugConsoleTab.render();
    }
    sl::gui::endPanel();
}
}
