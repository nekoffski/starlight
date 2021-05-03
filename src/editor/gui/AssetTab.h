#pragma once

#include <memory>

#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gui/GuiApi.h"

#include "Widget.h"

namespace editor::gui {

class AssetTab : public Widget {
public:
    explicit AssetTab(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState) {
    }

    void render(sl::gui::GuiApi& gui) override {
        if (gui.beginTabItem(ICON_FA_CLOUD "  Asset")) {
            if (auto& activeAssetGui = m_sharedState->activeAssetGui; activeAssetGui != nullptr)
                activeAssetGui->render(gui);

            gui.endTabItem();
        }
    }

private:
};
}
