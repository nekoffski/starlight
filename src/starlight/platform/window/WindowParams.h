#pragma once

#include <string>

#include <starlight/platform/window/Viewport.h>

namespace starl::platform::window {

struct WindowParams {
    Viewport viewport;
    std::string title;
};
}