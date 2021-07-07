#pragma once

#include <memory>

#include "Window.h"

namespace sl::core {

class WindowProxy {
public:
    explicit WindowProxy(std::shared_ptr<core::Window> window)
        : m_window(window) {
    }

    Window::Size getSize() const {
        return m_window->getSize();
    }

    void quit() {
        m_window->setShouldClose(true);
    }

private:
    std::shared_ptr<core::Window> m_window;
};
}