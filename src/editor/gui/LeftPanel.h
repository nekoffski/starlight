#pragma once

#include "sl/gui/GuiApi.h"

#include "PropertiesPanel.h"
#include "ScenePanel.h"
#include "Settings.h"
#include "Widget.h"

namespace editor::gui {

class LeftPanel : public Widget {
public:
    explicit LeftPanel(const Settings& settings, sl::asset::AssetManager& assetManager)
        : m_selectedEntity(nullptr)
        , m_scenePanel(settings.scenePanelPosition, m_selectedEntity)
        , m_propertiesPanel(settings.propertiesPanelPosition, assetManager, m_selectedEntity) {
    }

    void render(sl::gui::GuiApi& gui) override {
        m_scenePanel.render(gui);
        m_propertiesPanel.render(gui);
    }

    void setSettings(const Settings& settings) {
        m_scenePanel.setPosition(settings.scenePanelPosition);
        m_propertiesPanel.setPosition(settings.propertiesPanelPosition);
    }

    void setActiveScene(std::shared_ptr<sl::scene::Scene> scene) override {
        Widget::setActiveScene(scene);
        m_scenePanel.setActiveScene(scene);
        m_propertiesPanel.setActiveScene(scene);
    }

private:
    std::shared_ptr<sl::ecs::Entity> m_selectedEntity;

    ScenePanel m_scenePanel;
    PropertiesPanel m_propertiesPanel;
};
}
