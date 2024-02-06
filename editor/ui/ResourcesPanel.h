#pragma once

#include "UIState.h"

class ResourcesPanel {
public:
    explicit ResourcesPanel(UIState* state);

    void render();

private:
    UIState* m_state;

    sl::ui::TabMenu m_resourcesTab;
};
