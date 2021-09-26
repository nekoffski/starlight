#pragma once

#include "sl/platform/IO.h"

#include "GlfwInput.h"
#include "GlfwWindow.h"

#include "sl/cfg/Config.h"

namespace sl::platform::glfw {

struct Glfw : IO {
    explicit Glfw(const cfg::Config::Window& windowConfig)
        : IO(
              std::make_unique<GlfwWindow>(
                  math::Size2D { windowConfig.width, windowConfig.height }, windowConfig.name),
              std::make_unique<GlfwInput>()) { }
};

}