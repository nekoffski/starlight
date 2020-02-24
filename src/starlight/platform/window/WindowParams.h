#pragma once

#include <string>

namespace starl::platform::window {

struct WindowParams {
    unsigned int width;
    unsigned int height;
    std::string title;
};
}