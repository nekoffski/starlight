#pragma once

#include "BottomPanel.h"
#include "LeftPanel.h"
#include "Settings.h"

using namespace sl::core;

namespace editor::gui {

class EditorGui {

public:
    explicit EditorGui(const Settings& settings, EntitiesVector& entities, res::ResourceManager& resourceManager);
    void setSettings(const Settings& settings);
    void renderEditorGui(sl::gui::GuiApi& gui);

    void setActiveScene(std::shared_ptr<sl::scene::Scene> scene) {
		m_leftPanel.setActiveScene(scene);
		m_bottomPanel.setActiveScene(scene);
	}

private:
    LeftPanel m_leftPanel;
    BottomPanel m_bottomPanel;
};
}
