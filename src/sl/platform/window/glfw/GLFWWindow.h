#pragma once

#include <GLFW/glfw3.h>

#include "sl/platform/Error.h"
#include "sl/platform/window/Window.h"

namespace sl::platform::window::glfw {

class GLFWWindow : public Window {
public:
    using Window::Window;

    ~GLFWWindow() override;

    void init() override;
    void update(float) override;

    void* getHandle() const override {
        return static_cast<void*>(m_windowHandle);
    }

    void enableCursor() override;
    void disableCursor() override;

    bool getShouldClose() const override;
    void setShouldClose(bool) override;

private:
    GLFWwindow* m_windowHandle{ nullptr };
};
}
