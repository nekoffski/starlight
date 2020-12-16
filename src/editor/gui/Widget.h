#pragma once

#include "sl/gui/GuiApi.h"
#include "sl/math/Vector.hpp"

namespace editor::gui {

class Widget {
public:
    virtual void render(sl::gui::GuiApi& gui) = 0;
};

struct WidgetPosition {
    sl::math::Vec2 origin;
    sl::math::Vec2 size;
};
}
