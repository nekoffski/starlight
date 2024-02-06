#pragma once

#include "starlight/scene/Scene.h"

#include "UIState.h"

class ScenePanel {
public:
    explicit ScenePanel(sl::Scene* scene, UIState* state);

    void render();

private:
    sl::Scene* m_scene;
    UIState* m_state;
};
