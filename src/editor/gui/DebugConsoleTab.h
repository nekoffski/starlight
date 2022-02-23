#pragma once

#include "Widget.h"

namespace editor::gui {

class DebugConsoleTab : public Widget {
    using Widget::Widget;

   public:
    void render() override;
};

}  // namespace editor::gui
