#pragma once

#include "sl/gui/GUIProxy.h"
#include "sl/math/Vector.hpp"

namespace editor::gui {

class Widget {
public:
    virtual void render(sl::gui::GUIProxy& gui) = 0;
};

struct WidgetPosition {
    sl::math::Vec2 origin;
    sl::math::Vec2 size;
};
}
