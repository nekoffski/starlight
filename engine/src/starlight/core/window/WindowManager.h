#pragma once

#include <kc/core/Singleton.hpp>

#include "Window.h"

namespace sl {

class WindowManager : public kc::core::Singleton<WindowManager> {
   public:
    explicit WindowManager(Window* window);

    Size2u32 getSize() const;
    Vec2f getMousePosition() const;
    Vec2f getMousePositionDelta() const;

    void update();

    bool isKeyPressed(Window::Key keyCode) const;

   private:
    void calculateMousePositionDelta();

    void setCallbacks();

    Window* m_window;

    Vec2f m_previousMousePosition;
    Vec2f m_mousePositionDelta;
};

}  // namespace sl
