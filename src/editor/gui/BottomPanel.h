#pragma once

#include "Widget.h"

#include "AssetsTab.h"
#include "DebugConsoleTab.h"
#include "Settings.h"
#include "sl/asset/AssetManager.h"

namespace editor::gui {

class BottomPanel : public Widget {
public:
    explicit BottomPanel(const Settings& settings, sl::asset::AssetManager& assetManager);
    void render(sl::gui::GuiApi& gui) override;

	void setSettings(const Settings& settings) {
		m_position = settings.bottomPanelPosition;
	}

private:
    WidgetPosition m_position;
    AssetsTab m_assetsTab;
	DebugConsoleTab m_debugConsoleTab;

};
}
