#pragma once

#include "sl/platform/IO.h"

#include "GlfwInput.h"
#include "GlfwWindow.h"

namespace sl::platform::glfw {

struct Glfw : IO {
    explicit Glfw()
        : IO(
              std::make_unique<GlfwWindow>(math::Size2D { 1600, 900 }, "Starlight"),
              std::make_unique<GlfwInput>()) { }
};

}