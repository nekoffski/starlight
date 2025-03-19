#include "Events.hh"

namespace sl {

std::string toString(const WindowResized& event) {
    return fmt::format("WindowResized[{}/{}]", event.size.w, event.size.h);
}

std::string toString(const KeyAction& action) {
    switch (action) {
        case sl::KeyAction::press:
            return "KeyPress";

        case sl::KeyAction::repeat:
            return "KeyRepeat";

        case sl::KeyAction::release:
            return "KeyRelease";

        default:
            return "KeyUnknownAction";
    }
}

std::string toString(const MouseAction& action) {
    switch (action) {
        case sl::MouseAction::press:
            return "KeyPress";

        case sl::MouseAction::release:
            return "KeyRelease";
        default:
            return "KeyUnknownAction";
    }
}

std::string toString(const KeyEvent& event) {
    return fmt::format("KeyEvent[{}/{}]", event.key, event.action);
}

std::string toString(const MouseEvent& event) {
    return fmt::format(
      "MouseEvent[{}/{}/{}/{}]", event.button, event.action, event.position.x,
      event.position.y
    );
}

std::string toString(const ScrollEvent& event) {
    return fmt::format("ScrollEvent[{}]", event.offset);
}

}  // namespace sl
