#pragma once

#include <utility>

namespace sl::core {

struct Mouse {
    using Callback = void (*)(void*, double, double);

    virtual bool isMouseButtonPressed(int) const = 0;

    virtual std::pair<double, double> getMousePosition() const = 0;

    virtual void setOnMousePositionChange(Callback) = 0;
    virtual void setOnMouseScroll(Callback) = 0;
};

}