#pragma once

#include "sl/platform/IO.h"

#include "GlfwInput.h"
#include "GlfwWindow.h"

#include "sl/utils/Config.h"

namespace sl::platform::glfw {

struct Glfw : IO {
    explicit Glfw(const utils::Config::Window& windowConfig)
        : IO(
              std::make_unique<GlfwWindow>(
                  math::Size2D { windowConfig.width, windowConfig.height }, windowConfig.name),
              std::make_unique<GlfwInput>()) { }
};

}