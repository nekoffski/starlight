#pragma once

#include "Widget.h"

#include "AssetsTab.h"
#include "DebugConsoleTab.h"
#include "sl/asset/AssetManager.h"

namespace editor::gui {

class BottomPanel : public Widget {
public:
    explicit BottomPanel(std::shared_ptr<SharedState>);
    void render(sl::gui::GuiApi& gui) override;

private:
    AssetsTab m_assetsTab;
    DebugConsoleTab m_debugConsoleTab;
};
}
