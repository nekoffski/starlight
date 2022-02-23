#pragma once

#include "AssetsTab.h"
#include "DebugConsoleTab.h"
#include "Widget.h"
#include "sl/asset/AssetManager.h"

namespace editor::gui {

class BottomPanel : public Widget {
   public:
    explicit BottomPanel(std::shared_ptr<SharedState>);
    void render() override;

   private:
    AssetsTab m_assetsTab;
    DebugConsoleTab m_debugConsoleTab;
};
}  // namespace editor::gui
