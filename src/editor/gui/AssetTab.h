#pragma once

#include <memory>

#include "sl/ecs/Entity.h"
#include "sl/gfx/Cubemap.h"
#include "sl/gui/GuiApi.h"

#include <imgui_sugar.hpp>

#include "Widget.h"

namespace editor::gui {

class AssetTab : public Widget {
public:
    explicit AssetTab(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState) {
    }

    void render() override {
        with_TabItem(ICON_FA_CLOUD "  Asset") {
            if (auto& activeAssetGuiProvider = m_sharedState->activeAssetGuiProvider; activeAssetGuiProvider != nullptr)
                activeAssetGuiProvider->render();
        }
    }
};
}
