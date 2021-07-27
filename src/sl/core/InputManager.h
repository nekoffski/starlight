#pragma once

#include <memory>

#include "Keyboard.h"
#include "Logger.h"
#include "Macros.h"
#include "Mouse.h"
#include "Singleton.hpp"

#include "sl/platform/core/GlfwKeys.h"

namespace sl::core {

class InputManager : public Singleton<InputManager> {
public:
    // clang-format off
    InputManager& setMouse(Mouse* mouse);
    InputManager& setKeyboard(Keyboard* keyboard);
    // clang-format on

    void update();

    bool isMouseButtonPressed(int buttonCode) const;

    std::pair<double, double> getMousePosition() const;
    std::pair<double, double> getMousePositonDelta() const;

    double getScrollDelta() const;

    bool isKeyPressed(int keyCode) const;

private:
    Keyboard* m_keyboard;
    Mouse* m_mouse;
};

}
