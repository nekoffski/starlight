#pragma once

#include <string>

#include <kc/core/Macros.h>

namespace nova::core {

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

}  // namespace nova::core
