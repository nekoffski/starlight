#pragma once

#include <memory>

#include "Singleton.hpp"
#include "Window.h"

namespace sl::core {

class WindowManager : public Singleton<WindowManager> {
public:
    using Ptr = std::unique_ptr<WindowManager>;

    void setActiveWindow(Window* window) {
        m_window = window;
    }

    void enableCursor() {
        m_window->enableCursor();
    }

    void disableCursor() {
        m_window->disableCursor();
    }

    Window::Size getSize() const {
        return m_window->getSize();
    }

private:
    Window* m_window;
};

}