#pragma once

#include <optional>

#include "starlight/core/Core.h"
#include "starlight/ui/UI.h"

#include "starlight/renderer/RendererFrontend.h"

#include "starlight/scene/Scene.h"

#include "ScenePanel.h"
#include "EntityInspectorPanel.h"
#include "ResourceInspectorPanel.h"
#include "ResourcesPanel.h"
#include "Console.h"

#include "Core.h"

class UI {
public:
    explicit UI(
      sl::u64 w, sl::u64 h, sl::RendererFrontend& renderer, sl::Scene* scene
    );

    void setScene(sl::Scene* scene);

    void render();
    bool shouldExit() const;

    UIState* getState();
    Logger* getLogger();

private:
    sl::u64 m_width;
    sl::u64 m_height;

    sl::Scene* m_scene;

    Console m_console;
    Logger* m_logger;

    sl::ui::MainMenuBar m_mainMenu;

    sl::ui::PanelCombo m_leftCombo;
    sl::ui::PanelCombo m_rightCombo;
    sl::ui::PanelCombo m_bottomCombo;

    UIState m_state;

    ScenePanel m_scenePanel;
    ResourceInspectorPanel m_resourceInspectorPanel;
    EntityInspectorPanel m_entityInspectorPanel;
    ResourcesPanel m_resourcesPanel;

    bool m_shouldExit;
};
