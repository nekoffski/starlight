#pragma once

#include <kc/core/Log.h>

#include <kc/core/Singleton.hpp>
#include <memory>

#include "Keyboard.h"
#include "Macros.h"
#include "Mouse.h"
#include "sl/math/Vector.hpp"
#include "sl/platform/glfw/GlfwKeys.h"

namespace sl::core {

class InputManager : public kc::core::Singleton<InputManager> {
   public:
    void setMouse(Mouse* mouse);
    void setKeyboard(Keyboard* keyboard);

    void update();

    bool isMouseButtonPressed(int buttonCode) const;

    math::Vec2 getMousePosition() const;
    math::Vec2 getMousePositonDelta() const;

    double getScrollDelta() const;

    bool isKeyPressed(int keyCode) const;

   private:
    Keyboard* m_keyboard;
    Mouse* m_mouse;
};

}  // namespace sl::core
