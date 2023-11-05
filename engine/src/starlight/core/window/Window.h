#pragma once

#include <string_view>

#include "starlight/core/math/Size.hpp"
#include "starlight/core/math/Glm.h"

#include "starlight/core/event/Input.h"

namespace sl {

struct Window {
    using Key    = int;
    using Button = int;

    using OnKeyCallback          = void (*)(KeyAction, Key);
    using OnMouseCallback        = void (*)(MouseAction, Button);
    using OnWindowCloseCallback  = void (*)();
    using OnWindowResizeCallback = void (*)(uint32_t, uint32_t);

    virtual std::string_view getVendor() const = 0;

    virtual bool isKeyPressed(Key keyCode) const = 0;

    virtual void update()      = 0;
    virtual void swapBuffers() = 0;

    virtual void showCursor() = 0;
    virtual void hideCursor() = 0;

    virtual Size2u32 getSize() const       = 0;
    virtual Vec2f getMousePosition() const = 0;

    virtual void onKeyCallback(OnKeyCallback)                   = 0;
    virtual void onMouseCallback(OnMouseCallback)               = 0;
    virtual void onWindowCloseCallback(OnWindowCloseCallback)   = 0;
    virtual void onWindowResizeCallback(OnWindowResizeCallback) = 0;

    virtual void* getHandle() { return nullptr; }
};

}  // namespace sl