#pragma once

#include "sl/gui/GuiProxy.h"

#include "PropertiesPanel.h"
#include "ScenePanel.h"
#include "Settings.h"
#include "Widget.h"

namespace editor::gui {

class LeftPanel : public Widget {
public:
    explicit LeftPanel(const Settings& settings, EntitiesVector& entities, res::ResourceManager& resourceManager)
        : m_selectedEntity(nullptr)
        , m_scenePanel(settings.scenePanelPosition, entities, m_selectedEntity)
        , m_propertiesPanel(settings.propertiesPanelPosition, resourceManager, m_selectedEntity) {
    }

    void render(sl::gui::GuiProxy& gui) override {
        m_scenePanel.render(gui);
        m_propertiesPanel.render(gui);
    }

private:
    std::shared_ptr<sl::ecs::Entity> m_selectedEntity;

    ScenePanel m_scenePanel;
    PropertiesPanel m_propertiesPanel;
};
}
