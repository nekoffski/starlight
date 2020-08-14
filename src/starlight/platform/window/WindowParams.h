#pragma once

#include <string>

#include "Viewport.h"

namespace sl::platform::window {

struct WindowParams {
    Viewport viewport;
    std::string title;
};
}