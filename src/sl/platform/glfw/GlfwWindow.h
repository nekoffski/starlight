#pragma once

#include <GLFW/glfw3.h>

#include "sl/core/Window.h"

namespace sl::platform::glfw {

class GlfwWindow : public core::Window {
public:
    using core::Window::Window;

    ~GlfwWindow() override;

    void init() override;
    void update(float) override;

    void* getHandle() const override;

    void setResizeCallback(ResizeCallback) override;

    void enableCursor() override;
    void disableCursor() override;
    void changeCursorState(bool) override;

    bool getShouldClose() const override;
    void setShouldClose(bool) override;

    void makeContextCurrent() override;
    void swapBuffers() override;

    core::Window::Size getSize() const override;

private:
    GLFWwindow* m_windowHandle = nullptr;
};

}
