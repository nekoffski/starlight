#pragma once

#include <memory>

#include "sl/core/Input.h"
#include "sl/core/Window.h"

namespace sl::platform {

struct IO {
    explicit IO(std::unique_ptr<core::Window> window, std::unique_ptr<core::Input> input)
        : window(std::move(window))
        , input(std::move(input)) {
    }

    std::unique_ptr<core::Window> window;
    std::unique_ptr<core::Input> input;
};

}