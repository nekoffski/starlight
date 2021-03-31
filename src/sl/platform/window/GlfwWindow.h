#pragma once

#include <GLFW/glfw3.h>

#include "sl/core/Window.h"

namespace sl::platform::window {

class GlfwWindow : public core::Window {
public:
    using core::Window::Window;

    ~GlfwWindow() override;

    void init() override;
    void update(float) override;

    void* getHandle() const override {
        return static_cast<void*>(m_windowHandle);
    }

    void setResizeCallback(ResizeCallback) override;

    void enableCursor() override;
    void disableCursor() override;
    void changeCursorState(bool) override;

    bool getShouldClose() const override;
    void setShouldClose(bool) override;

private:
    GLFWwindow* m_windowHandle { nullptr };
};

struct GlfwWindowFactory : core::Window::Factory {
    std::shared_ptr<core::Window> create(core::Window::Size windowSize, const std::string& title) {
        return std::make_shared<GlfwWindow>(windowSize, title);
    }
};
}
