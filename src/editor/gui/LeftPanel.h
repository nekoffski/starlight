#pragma once

#include "sl/gui/GUIProxy.h"

#include "PropertiesPanel.h"
#include "ScenePanel.h"
#include "Settings.h"
#include "Widget.h"

namespace editor::gui {

class LeftPanel : public Widget {
public:
    explicit LeftPanel(const Settings& settings, EntitiesVector& entities, res::ResourceManager& resourceManager)
        : m_scenePanel(settings.scenePanelPosition, entities)
        , m_propertiesPanel(settings.propertiesPanelPosition, resourceManager) {
    }

    void render(sl::gui::GUIProxy& gui) override {
        m_scenePanel.render(gui);
        m_propertiesPanel.render(gui);
    }

private:
    ScenePanel m_scenePanel;
    PropertiesPanel m_propertiesPanel;
};
}
