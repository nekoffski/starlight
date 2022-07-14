#pragma once

#include "nova/core/Window.h"

namespace nova::platform::glfw {

class GlfwWindow : public core::Window {
    struct Callbacks {
        OnWindowCloseCallback onWindowClose;
        OnKeyCallback onKey;
        OnMouseCallback onMouse;
        OnWindowResizeCallback onWindowResize;
    };

   public:
    explicit GlfwWindow();

    std::string_view getVendor() const override;
    math::Size2u32 getSize() const override;
    math::Vec2f getMousePosition() const override;

    void update() override;
    void swapBuffers() override;

    void onKeyCallback(OnKeyCallback) override;
    void onMouseCallback(OnMouseCallback) override;
    void onWindowCloseCallback(OnWindowCloseCallback) override;
    void onWindowResizeCallback(OnWindowResizeCallback) override;

    void* getHandle() override;

   private:
    void* m_windowHandle;
    Callbacks m_callbacks;
};

}  // namespace nova::platform::glfw