#include "WindowManager.h"

#include <kc/core/Log.h>

#include "starlight/core/event/Event.h"
#include "starlight/core/event/Quit.h"
#include "starlight/core/event/Input.h"
#include "starlight/core/event/WindowResized.h"

namespace sl {

WindowManager::WindowManager(Window* window)
    : m_window(window), m_previousMousePosition(m_window->getMousePosition()) {
    setCallbacks();
}

bool WindowManager::isKeyPressed(Window::Key keyCode) const {
    return m_window->isKeyPressed(keyCode);
}

void WindowManager::setCallbacks() {
    m_window->onWindowCloseCallback([]() {
        LOG_INFO("Window closed, emitting event");
        EventManager::get().emit<QuitEvent>("Window closed");
    });

    m_window->onKeyCallback([](KeyAction action, int key) {
        KeyEvent event{action, key};
        LOG_TRACE("Detected key action, emitting event: {}", event);
        EventManager::get().emit<KeyEvent>(event);
    });

    m_window->onMouseCallback([](MouseAction action, int button) {
        MouseEvent event{action, button};
        LOG_TRACE("Detected mouse action, emitting event: {}", event);
        EventManager::get().emit<MouseEvent>(event);
    });

    m_window->onWindowResizeCallback([](uint32_t width, uint32_t height) {
        WindowResized event{width, height};
        LOG_TRACE("Window resized, emitting event: {}", event);
        EventManager::get().emit<WindowResized>(event);
    });
}

Size2u32 WindowManager::getSize() const { return m_window->getSize(); }

Vec2f WindowManager::getMousePosition() const { return m_window->getMousePosition(); }

Vec2f WindowManager::getMousePositionDelta() const { return m_mousePositionDelta; }

void WindowManager::update() { calculateMousePositionDelta(); }

void WindowManager::calculateMousePositionDelta() {
    auto mousePosition      = m_window->getMousePosition();
    m_mousePositionDelta    = mousePosition - m_previousMousePosition;
    m_previousMousePosition = mousePosition;
}

}  // namespace sl
