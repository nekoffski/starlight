#include "WindowManager.h"

#include <kc/core/Log.h>

#include "nova/event/Event.h"
#include "nova/event/Quit.h"
#include "nova/event/Input.h"

namespace nova::core {

WindowManager::WindowManager(Window* window)
    : m_window(window), m_previousMousePosition(m_window->getMousePosition()) {
    setCallbacks();
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
}

math::Size2i WindowManager::getSize() const { return m_window->getSize(); }

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
