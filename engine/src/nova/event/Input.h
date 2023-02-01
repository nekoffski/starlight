#pragma once

#include <string>

#include <kc/core/Log.h>
#include <kc/core/Macros.h>

#include "nova/core/Input.hpp"
#include "nova/platform/Keys.h"

namespace nova::event {

struct KeyEvent {
    core::KeyAction action;
    int key;
};

struct MouseEvent {
    core::MouseAction action;
    int button;
};

inline std::string toString(const KeyEvent& event) {
    return fmt::format("KeyEvent[{}/{}]", event.key, event.action);
}

inline std::string toString(const MouseEvent& event) {
    return fmt::format("MouseEvent[{}/{}]", event.button, event.action);
}

TO_STRING_STREAM(KeyEvent);
TO_STRING_STREAM(MouseEvent);

}  // namespace nova::event
