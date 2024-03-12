#pragma once

#include "starlight/scene/Scene.h"

#include "Core.h"

#include "starlight/ui/UI.h"

#include "Console.h"

class ScenePanel {
public:
    explicit ScenePanel(sl::Scene* scene, UIState* state, Logger* logger);

    void render();

private:
    void renderSceneGraph();
    void renderSceneSettings();

    sl::Scene* m_scene;
    UIState* m_state;
    Logger* m_logger;

    sl::ui::TabMenu m_tabs;
};
