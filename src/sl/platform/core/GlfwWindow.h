#pragma once

#include <GLFW/glfw3.h>

#include "sl/core/Window.h"

namespace sl::platform::core {

class GlfwWindow : public sl::core::Window {
public:
    using sl::core::Window::Window;

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

struct GlfwWindowFactory : sl::core::Window::Factory {
    std::unique_ptr<sl::core::Window> create(sl::core::Window::Size windowSize, const std::string& title) {
        return std::make_unique<GlfwWindow>(windowSize, title);
    }
};
}
