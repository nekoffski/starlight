#pragma once

#include "Widget.h"

#include "sl/gui/GuiApi.h"

namespace editor::gui {

class DebugConsoleTab : public Widget {
    using Widget::Widget;

public:
    void render(sl::gui::GuiApi& gui) override;
};

}
