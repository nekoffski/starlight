#pragma once

#include "BottomPanel.h"
#include "LeftPanel.h"
#include "Settings.h"
#include "sl/asset/AssetManager.h"

using namespace sl::core;

namespace editor::gui {

class EditorGui {

public:
    explicit EditorGui(const Settings& settings, sl::asset::AssetManager& assetManager);
    void setSettings(const Settings& settings);
    void renderEditorGui(sl::gui::GuiApi& gui);

    void setActiveScene(std::shared_ptr<sl::scene::Scene> scene) {
        m_leftPanel.setActiveScene(scene);
        m_bottomPanel.setActiveScene(scene);
    }

private:
    LeftPanel m_leftPanel;
    BottomPanel m_bottomPanel;
    sl::asset::AssetManager& m_assetManager;
};
}
