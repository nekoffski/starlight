#pragma once



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

    void render() override {
        auto& widgetProperties = m_sharedState->guiProperties.rightPanelProperties;
        sl::gui::beginPanel(" " ICON_FA_WRENCH " Inspector", widgetProperties.origin, widgetProperties.size);

        with_TabBar("lowerLeftTabBar")
            m_entityTab.render();

        sl::gui::endPanel();
    }

private:
    EntityTab m_entityTab;
};
}
