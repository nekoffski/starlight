#pragma once

#include "starlight/core/Core.h"
#include "starlight/ui/UI.h"

#include "starlight/renderer/RendererFrontend.h"

class UI {
public:
    explicit UI(sl::u64 w, sl::u64 h, sl::RendererFrontend& renderer);

    void render();

private:
    sl::u64 m_width;
    sl::u64 m_height;

    sl::ui::MainMenuBar m_mainMenu;
    sl::ui::PanelCombo m_leftCombo;
};
