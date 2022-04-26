#pragma once

#include <utility>

#include "sl/math/Vector.hpp"

namespace sl::core {

struct Mouse {
    using Callback = void (*)(void*, double, double);

    virtual bool isMouseButtonPressed(int) const = 0;

    virtual math::Vec2 getMousePosition() const = 0;

    virtual void setOnMousePositionChange(Callback) = 0;
    virtual void setOnMouseScroll(Callback)         = 0;
};

}  // namespace sl::core