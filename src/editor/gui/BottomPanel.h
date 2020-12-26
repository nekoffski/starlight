#pragma once

#include "Widget.h"

#include "ResourcesTab.h"
#include "Settings.h"
#include "editor/res/ResourceManager.h"

namespace editor::gui {

class BottomPanel : public Widget {
public:
    explicit BottomPanel(const Settings& settings, res::ResourceManager& resourceManager);
    void render(sl::gui::GuiApi& gui) override;

	void setSettings(const Settings& settings) {
		m_position = settings.bottomPanelPosition;
	}

private:
    WidgetPosition m_position;
    ResourcesTab m_resourcesTab;
};
}
