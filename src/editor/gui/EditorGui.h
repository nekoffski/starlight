#pragma once

#include "BottomPanel.h"
#include "LeftPanel.h"
#include "RightPanel.h"
#include "SharedState.hpp"
#include "ToolBar.h"
#include "sl/asset/AssetManager.h"
#include "sl/gui/FileBrowser.h"

using namespace sl::core;

namespace editor::gui {

class EditorGui {
   public:
    explicit EditorGui(std::shared_ptr<SharedState> sharedState);
    void renderEditorGui();

    std::shared_ptr<SharedState> sharedState;

   private:
    LeftPanel m_leftPanel;
    BottomPanel m_bottomPanel;
    RightPanel m_rightPanel;
    ToolBar m_toolBar;

    sl::gui::FileBrowser m_fileBrowser;
};
}  // namespace editor::gui
