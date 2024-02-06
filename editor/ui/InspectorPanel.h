#pragma once

#pragma once

#include "starlight/scene/Scene.h"

#include "UIState.h"

class InspectorPanel {
public:
    explicit InspectorPanel(sl::Scene* scene, UIState* state);

    void render();

private:
    void renderEntityUI(sl::u64 entityId);
    void renderResourceUI();

    sl::Scene* m_scene;
    UIState* m_state;
    sl::ui::TabMenu m_menu;

    sl::ui::Slider3 m_translationSlider;
    sl::ui::Slider3 m_scaleSlider;
    sl::ui::Slider3 m_orientationSlider;
};
