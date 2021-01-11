#pragma once

#include "Widget.h"

#include "sl/gui/GuiApi.h"

namespace editor::gui {

class DebugConsoleTab : public Widget {
public:
    void render(sl::gui::GuiApi& gui) override;
};

}

