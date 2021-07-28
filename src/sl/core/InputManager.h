#pragma once

#include <memory>

#include "Keyboard.h"
#include "Logger.h"
#include "Macros.h"
#include "Mouse.h"
#include "Singleton.hpp"

#include "sl/platform/glfw/GlfwKeys.h"

namespace sl::core {

class InputManager : public Singleton<InputManager> {
public:
    void setMouse(Mouse* mouse);
    void setKeyboard(Keyboard* keyboard);

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
