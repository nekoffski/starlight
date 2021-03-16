#pragma once

#include "BottomPanel.h"
#include "LeftPanel.h"
#include "RightPanel.h"
#include "SharedState.hpp"
#include "sl/asset/AssetManager.h"

using namespace sl::core;

namespace editor::gui {

class EditorGui {

public:
    explicit EditorGui(std::shared_ptr<SharedState> sharedState);
    void renderEditorGui(sl::gui::GuiApi& gui);

    std::shared_ptr<SharedState> sharedState;

private:
    LeftPanel m_leftPanel;
    BottomPanel m_bottomPanel;
    RightPanel m_rightPanel;
};
}
