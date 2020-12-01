#pragma once

#include "Widget.h"

#include "Settings.h"
#include "ResourcesTab.h"
#include "editor/res/ResourceManager.h"

namespace editor::gui {

class BottomPanel : public Widget {
public:
    explicit BottomPanel(const Settings& settings, res::ResourceManager& resourceManager);
    void render(sl::gui::GUIProxy& gui) override;

private:
    WidgetPosition m_position;
    ResourcesTab m_resourcesTab;
};
}
