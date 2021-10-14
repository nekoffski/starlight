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

    void render() override {
        m_scenePanel.render();
        m_propertiesPanel.render();
    }

private:
    ScenePanel m_scenePanel;
    PropertiesPanel m_propertiesPanel;
};
}
