#pragma once

#include <kc/core/Singleton.hpp>

#include "starlight/core/math/Core.h"
#include "Window.h"

namespace sl {

class WindowManager : public kc::core::Singleton<WindowManager> {
public:
    explicit WindowManager(Window* window);

    Vec2u32 getSize() const;
    Vec2u32 getFramebufferSize() const;
    Vec2f getMousePosition() const;
    Vec2f getMousePositionDelta() const;

    void showCursor();
    void hideCursor();

    void update();

    bool isKeyPressed(Window::Key keyCode) const;
    bool isMouseButtonPressed(Window::Button buttonCode) const;

private:
    void calculateMousePositionDelta();

    void setCallbacks();

    Window* m_window;

    Vec2f m_previousMousePosition;
    Vec2f m_mousePositionDelta;
};

}  // namespace sl
