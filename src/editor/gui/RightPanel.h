#pragma once

#include "sl/gui/GuiApi.h"

#include "AssetTab.h"
#include "EntityTab.h"
#include "Widget.h"

#include "sl/gui/fonts/FontAwesome.h"

namespace editor::gui {

class RightPanel : public Widget {
public:
    explicit RightPanel(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState)
        , m_entityTab(sharedState) {
    }

    void render(sl::gui::GuiApi& gui) override {
        auto& widgetProperties = m_sharedState->guiProperties.rightPanelProperties;
        gui.beginPanel(" " ICON_FA_WRENCH " Inspector", widgetProperties.origin, widgetProperties.size);
        gui.beginTabBar("lowerLeftTabBar");

        m_entityTab.render(gui);
        gui.endTabBar();
        gui.endPanel();
    }

private:
    EntityTab m_entityTab;
};
}
