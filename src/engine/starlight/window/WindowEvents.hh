#pragma once

#include "WindowTypes.hh"
#include "starlight/core/Core.hh"
#include "starlight/math/Math.hh"

namespace sl {

struct WindowResizedEvent {
    u32 width;
    u32 height;
    WindowId window{0};
};

struct WindowCloseRequestedEvent {
    WindowId window;
};

struct WindowFocusChangedEvent {
    WindowId window;
    bool focused;
};

struct KeyPressedEvent {
    WindowId window;
    Key key;
    KeyModifiers modifiers{};
    bool repeat{false};
};

struct KeyReleasedEvent {
    WindowId window;
    Key key;
    KeyModifiers modifiers{};
};

struct MouseButtonPressedEvent {
    WindowId window;
    MouseButton button;
    Vec2f position{0.0f};
};

struct MouseButtonReleasedEvent {
    WindowId window;
    MouseButton button;
    Vec2f position{0.0f};
};

struct MouseMovedEvent {
    WindowId window;
    Vec2f position{0.0f};
    Vec2f delta{0.0f};
};

struct MouseWheelEvent {
    WindowId window;
    Vec2f delta{0.0f};
};

}  // namespace sl
