#pragma once

#include "starlight/core/window/Window.h"

namespace sl::glfw {

class Window : public sl::Window {
    struct Callbacks {
        OnWindowCloseCallback onWindowClose;
        OnKeyCallback onKey;
        OnScrollCallback onScroll;
        OnMouseCallback onMouse;
        OnWindowResizeCallback onWindowResize;
    };

public:
    explicit Window();

    void showCursor() override;
    void hideCursor() override;

    std::string_view getVendor() const override;
    Size2u32 getSize() const override;
    Vec2f getMousePosition() const override;

    bool isKeyPressed(Window::Key keyCode) const override;
    bool isMouseButtonPressed(Window::Button buttonCode) const override;
    void update() override;
    void swapBuffers() override;

    void onKeyCallback(OnKeyCallback) override;
    void onMouseCallback(OnMouseCallback) override;
    void onScrollCallback(OnScrollCallback) override;
    void onWindowCloseCallback(OnWindowCloseCallback) override;
    void onWindowResizeCallback(OnWindowResizeCallback) override;

    void* getHandle() override;

private:
    void* m_windowHandle;
    Callbacks m_callbacks;
};

}  // namespace sl::glfw