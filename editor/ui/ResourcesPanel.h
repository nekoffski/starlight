#pragma once

#include "Core.h"
#include "Console.h"

class ResourcesPanel {
public:
    explicit ResourcesPanel(UIState* state, Logger* logger);

    void render();

private:
    void renderLoadPopup();

    UIState* m_state;
    Logger* m_logger;

    sl::ui::TabMenu m_resourcesTab;
    sl::ui::Popup m_loadPopup;
};
