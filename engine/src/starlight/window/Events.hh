#pragma once

#include "starlight/core/math/Core.hh"
#include "starlight/core/Utils.hh"
#include "Keys.hh"

namespace sl {

enum class KeyAction : unsigned char { press, repeat, release, unknown };
enum class MouseAction : unsigned char { press, release, unknown };

struct QuitEvent {
    std::string reason;
};

struct KeyEvent {
    KeyAction action;
    int key;
};

struct MouseEvent {
    MouseAction action;
    int button;
    Vec2<f32> position;
};

struct ScrollEvent {
    float offset;
};

struct WindowResized {
    Vec2<u32> size;
};

std::string toString(const WindowResized& event);
std::string toString(const KeyAction& action);
std::string toString(const MouseAction& action);
std::string toString(const KeyEvent& event);
std::string toString(const MouseEvent& event);
std::string toString(const ScrollEvent& event);

}  // namespace sl
