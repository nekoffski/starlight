#pragma once

#include <kc/core/Singleton.hpp>

#include "starlight/core/math/Core.hh"
#include "Window.hh"

namespace sl {

class WindowManager : public kc::core::Singleton<WindowManager> {
public:
    explicit WindowManager(Window* window);

    Vec2<u32> getSize() const;
    Vec2<u32> getFramebufferSize() const;
    Vec2<f32> getMousePosition() const;
    Vec2<f32> getMousePositionDelta() const;

    void showCursor();
    void hideCursor();

    void update();

    bool isKeyPressed(Window::Key keyCode) const;
    bool isMouseButtonPressed(Window::Button buttonCode) const;

private:
    void calculateMousePositionDelta();

    void setCallbacks();

    Window* m_window;

    Vec2<f32> m_previousMousePosition;
    Vec2<f32> m_mousePositionDelta;
};

}  // namespace sl
