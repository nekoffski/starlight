#pragma once

#pragma once

#include "starlight/scene/Scene.h"

#include "Core.h"
#include "Console.h"

#include "starlight/renderer/gpu/Texture.h"

class InspectorPanel {
public:
    explicit InspectorPanel(sl::Scene* scene, UIState* state, Logger* logger);

    void render();

private:
    void renderEntityUI(sl::u64 entityId);
    void renderResourceUI(sl::u64 resourceId, ResourceType type);

    void renderTextureUI(sl::Texture* texture);

    sl::Scene* m_scene;
    UIState* m_state;
    Logger* m_logger;

    sl::ui::TabMenu m_menu;
    sl::ui::Slider3 m_translationSlider;
    sl::ui::Slider3 m_scaleSlider;
    sl::ui::Slider3 m_orientationSlider;
};
