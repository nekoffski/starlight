#pragma once

#include <functional>
#include <memory>
#include <tuple>

#include "sl/core/types/NotNullPtr.hpp"
#include "sl/platform/PlatformDetector.h"

#ifdef STARL_USE_GLFW
#include "sl/platform/input/GlfwKeys.h"
#endif

namespace sl::core {

using InputCallback = std::function<void(int)>;

class Input {
public:
    struct Factory {
        virtual std::shared_ptr<Input> create(core::types::NotNullPtr<void>) = 0;
        virtual ~Factory() = default;
    };

    virtual ~Input() = default;

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual void update() = 0;
    virtual void setKeyCallback(InputCallback) {};

    virtual bool isKeyPressed(int) const = 0;
    virtual bool isMouseButtonPressed(int) const = 0;

    virtual std::pair<double, double> getMousePosition() const = 0;
    virtual std::pair<double, double> getMousePositonDelta() const = 0;
    virtual double getScrollDelta() const = 0;
};
} // namespace sl::core
