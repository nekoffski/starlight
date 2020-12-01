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
        : m_panelWidth(settings.viewportWidth * settings.leftPanelWidthRatio)
        , m_scenePanelHeight(settings.viewportHeight * settings.leftSideTopBottomRatio)
        , m_propertiesPanelHeight(settings.viewportHeight - m_scenePanelHeight)
        , m_scenePanel({ { 0, 0 }, { m_panelWidth, m_scenePanelHeight } }, entities)
        , m_propertiesPanel({ { 0, m_scenePanelHeight }, { m_panelWidth, m_propertiesPanelHeight } }, resourceManager) {
    }

    void render(sl::gui::GUIProxy& gui) override {
        m_scenePanel.render(gui);
        m_propertiesPanel.render(gui);
    }

private:
    int m_panelWidth;
    int m_scenePanelHeight;
    int m_propertiesPanelHeight;

    ScenePanel m_scenePanel;
    PropertiesPanel m_propertiesPanel;
};
}
