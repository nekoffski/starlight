#pragma once

#include "sl/gui/GuiApi.h"

#include "PropertiesPanel.h"
#include "ScenePanel.h"
#include "Widget.h"

namespace editor::gui {

class LeftPanel : public Widget {
public:
    explicit LeftPanel(std::shared_ptr<SharedState> sharedState)
        : Widget(sharedState)
        , m_scenePanel(sharedState)
        , m_propertiesPanel(sharedState) {
    }

    void render(sl::gui::GuiApi& gui) override {
        m_scenePanel.render(gui);
        m_propertiesPanel.render(gui);
    }

private:
    ScenePanel m_scenePanel;
    PropertiesPanel m_propertiesPanel;
};
}
