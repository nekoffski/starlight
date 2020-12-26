#pragma once

#include <functional>
#include <memory>
#include <tuple>

#include "sl/misc/types/NotNullPtr.hpp"
#include "sl/platform/PlatformDetector.h"

#ifdef STARL_USE_GLFW
#include "sl/core/GlfwInput.h"
#endif

namespace sl::core {

using InputCallback = std::function<void(int)>;

class Input {
public:
    virtual void setKeyCallback(InputCallback){};

    virtual bool isKeyPressed(int) const = 0;
    virtual bool isMouseButtonPressed(int) const = 0;
    virtual std::pair<double, double> getMousePosition() const = 0;
};
} // namespace sl::core
