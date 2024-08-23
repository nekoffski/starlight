#pragma once

#include <string>

#include <fmt/core.h>
#include <kc/core/Macros.h>

#include "starlight/core/window/Keys.hh"

namespace sl {

enum class KeyAction : unsigned char { press, repeat, release, unknown };
enum class MouseAction : unsigned char { press, release, unknown };

inline std::string toString(KeyAction action) {
    switch (action) {
        case KeyAction::press:
            return "KeyPress";

        case KeyAction::repeat:
            return "KeyRepeat";

        case KeyAction::release:
            return "KeyRelease";
    }
    return "KeyUnknownAction";
}

inline std::string toString(MouseAction action) {
    switch (action) {
        case MouseAction::press:
            return "KeyPress";

        case MouseAction::release:
            return "KeyRelease";
    }
    return "KeyUnknownAction";
}

TO_STRING_STREAM(KeyAction);
TO_STRING_STREAM(MouseAction);

struct KeyEvent {
    KeyAction action;
    int key;
};

struct MouseEvent {
    MouseAction action;
    int button;
};

struct ScrollEvent {
    float offset;
};

inline std::string toString(const KeyEvent& event) {
    return fmt::format("KeyEvent[{}/{}]", event.key, event.action);
}

inline std::string toString(const MouseEvent& event) {
    return fmt::format("MouseEvent[{}/{}]", event.button, event.action);
}

inline std::string toString(const ScrollEvent& event) {
    return fmt::format("ScrollEvent[{}]", event.offset);
}

TO_STRING_STREAM(KeyEvent);
TO_STRING_STREAM(MouseEvent);
TO_STRING_STREAM(ScrollEvent);

}  // namespace sl
