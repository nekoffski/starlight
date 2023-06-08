#pragma once

#include <kc/core/Singleton.hpp>

#include "Window.h"

namespace sl::core {

class WindowManager : public kc::core::Singleton<WindowManager> {
   public:
    explicit WindowManager(Window* window);

    math::Size2u32 getSize() const;
    math::Vec2f getMousePosition() const;
    math::Vec2f getMousePositionDelta() const;

    void update();

    bool isKeyPressed(Window::Key keyCode) const;

   private:
    void calculateMousePositionDelta();

    void setCallbacks();

    Window* m_window;

    math::Vec2f m_previousMousePosition;
    math::Vec2f m_mousePositionDelta;
};

}  // namespace sl::core
