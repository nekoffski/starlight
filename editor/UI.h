#pragma once

#include <optional>

#include "starlight/core/Core.h"
#include "starlight/ui/UI.h"

#include "starlight/renderer/RendererFrontend.h"

#include "starlight/scene/Scene.h"

class UI {
public:
    explicit UI(
      sl::u64 w, sl::u64 h, sl::RendererFrontend& renderer, sl::Scene* scene
    );

    void setScene(sl::Scene* scene);

    void render();
    bool shouldExit() const;

private:
    sl::u64 m_width;
    sl::u64 m_height;

    sl::Scene* m_scene;

    sl::ui::MainMenuBar m_mainMenu;
    sl::ui::PanelCombo m_leftCombo;
    sl::ui::PanelCombo m_rightCombo;

    std::optional<sl::u64> m_selectedEntityId;

    bool m_shouldExit;
};
