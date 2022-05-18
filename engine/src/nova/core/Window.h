#pragma once

#include <string_view>

#include "nova/math/Size.hpp"
#include "Input.hpp"

namespace nova::core {

struct Window {
    using OnKeyCallback         = void (*)(KeyAction, int);
    using OnMouseCallback       = void (*)(MouseAction, int);
    using OnWindowCloseCallback = void (*)();

    virtual std::string_view getVendor() const = 0;
    virtual void update()                      = 0;
    virtual void swapBuffers()                 = 0;
    virtual math::Size2i getSize() const       = 0;

    virtual void onKeyCallback(OnKeyCallback)                 = 0;
    virtual void onMouseCallback(OnMouseCallback)             = 0;
    virtual void onWindowCloseCallback(OnWindowCloseCallback) = 0;
};

}  // namespace nova::core