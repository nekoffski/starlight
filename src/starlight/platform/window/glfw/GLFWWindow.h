#pragma once

#include <GLFW/glfw3.h>

#include <starlight/platform/Error.h>
#include <starlight/platform/window/Window.h>

namespace starl::platform::window::glfw {

class GLFWWindow : public Window {
public:
    using Window::Window;

    ~GLFWWindow() override;

    void init() override;
    void update(float) override;

    void* getHandle() const override {
        return static_cast<void*>(m_windowHandle);
    }

    bool getShouldClose() const override;
    void setShouldClose(bool) override;

private:
    GLFWwindow* m_windowHandle{ nullptr };
};
}