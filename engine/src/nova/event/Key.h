#pragma once

#include <string>

#include <kc/core/Log.h>
#include <kc/core/Macros.h>

namespace nova::event {

struct KeyEvent {
    enum class Action : unsigned char { press, repeat, release, unknown };

    Action action;
    int key;
};

inline std::string toString(KeyEvent::Action action) {
    switch (action) {
        case KeyEvent::Action::press:
            return "KeyPress";

        case KeyEvent::Action::repeat:
            return "KeyRepeat";

        case KeyEvent::Action::release:
            return "KeyRelease";
    }
    return "KeyUnknownAction";
}

TO_STRING_STREAM(KeyEvent::Action);

inline std::string toString(const KeyEvent& event) {
    return fmt::format("KeyEvent[{}/{}]", event.key, event.action);
}

TO_STRING_STREAM(KeyEvent);

}  // namespace nova::event
