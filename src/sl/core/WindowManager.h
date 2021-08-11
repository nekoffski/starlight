#pragma once

#include <memory>

#include "Singleton.hpp"
#include "Window.h"

namespace sl::core {

class WindowManager : public Singleton<WindowManager> {
public:
    void setActiveWindow(Window* window) {
        m_window = window;
    }

    void enableCursor(bool shouldEnable) {
        m_window->changeCursorState(shouldEnable);
    }

    void enableCursor() {
        m_window->enableCursor();
    }

    void disableCursor() {
        m_window->disableCursor();
    }

    math::Size2D getSize() const {
        return m_window->getSize();
    }

    void* getWindowHandle() const {
        return m_window->getHandle();
    }

private:
    Window* m_window;
};

}