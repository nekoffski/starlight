#pragma once

#include <functional>
#include <memory>
#include <tuple>

#include "starlight/misc/types/NotNullPtr.hpp"
#include "starlight/platform/PlatformDetector.h"

#ifdef STARL_USE_GLFW
#include "starlight/platform/input/glfw/GLFWKeys.h"
#endif

namespace sl::platform::input {

using InputCallback = std::function<void(int)>;

class Input {
public:
    static std::shared_ptr<Input> create(misc::types::NotNullPtr<void>);

    virtual void setKeyCallback(InputCallback){};

    virtual bool isKeyPressed(int) const = 0;
    virtual bool isMouseButtonPressed(int) const = 0;
    virtual std::pair<double, double> getMousePosition() const = 0;
};
} // namespace sl::platform::input
