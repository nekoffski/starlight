#include "WindowManager.h"

#include <kc/core/Log.h>

#include "nova/event/Event.h"
#include "nova/event/Quit.h"
#include "nova/event/Input.h"
#include "nova/event/WindowResized.h"

namespace nova::core {

WindowManager::WindowManager(Window* window)
    : m_window(window), m_previousMousePosition(m_window->getMousePosition()) {
    setCallbacks();
}

bool WindowManager::isKeyPressed(Window::Key keyCode) const {
    return m_window->isKeyPressed(keyCode);
}

void WindowManager::setCallbacks() {
    using namespace event;

    m_window->onWindowCloseCallback([]() {
        LOG_INFO("Window closed, emitting event");
        EventManager::get().emitEvent<QuitEvent>("Window closed");
    });

    m_window->onKeyCallback([](core::KeyAction action, int key) {
        KeyEvent event{action, key};
        LOG_TRACE("Detected key action, emitting event: {}", event);
        EventManager::get().emitEvent<KeyEvent>(event);
    });

    m_window->onMouseCallback([](core::MouseAction action, int button) {
        MouseEvent event{action, button};
        LOG_TRACE("Detected mouse action, emitting event: {}", event);
        EventManager::get().emitEvent<MouseEvent>(event);
    });

    m_window->onWindowResizeCallback([](uint32_t width, uint32_t height) {
        WindowResized event{width, height};
        LOG_TRACE("Window resized, emitting event: {}", event);
        EventManager::get().emitEvent<WindowResized>(event);
    });
}

math::Size2u32 WindowManager::getSize() const { return m_window->getSize(); }

math::Vec2f WindowManager::getMousePosition() const { return m_window->getMousePosition(); }

math::Vec2f WindowManager::getMousePositionDelta() const { return m_mousePositionDelta; }

void WindowManager::update() {
    m_window->update();
    calculateMousePositionDelta();
}

void WindowManager::calculateMousePositionDelta() {
    auto mousePosition      = m_window->getMousePosition();
    m_mousePositionDelta    = mousePosition - m_previousMousePosition;
    m_previousMousePosition = mousePosition;
}

}  // namespace nova::core
