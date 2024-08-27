#include "WindowManager.hh"

#include <kc/core/Log.h>

#include "starlight/core/event/EventProxy.hh"
#include "starlight/core/event/Quit.hh"
#include "starlight/core/event/Input.hh"
#include "starlight/core/event/WindowResized.hh"

namespace sl {

WindowManager::WindowManager(Window* window) :
    m_window(window), m_previousMousePosition(m_window->getMousePosition()) {
    setCallbacks();
}

bool WindowManager::isKeyPressed(Window::Key keyCode) const {
    return m_window->isKeyPressed(keyCode);
}

bool WindowManager::isMouseButtonPressed(Window::Button buttonCode) const {
    return m_window->isMouseButtonPressed(buttonCode);
}

void WindowManager::setCallbacks() {
    m_window->onWindowCloseCallback([]() {
        LOG_INFO("Window closed, emitting event");
        EventProxy::get().emit<QuitEvent>("Window closed");
    });

    m_window->onKeyCallback([](KeyAction action, int key) {
        KeyEvent event{ action, key };
        LOG_TRACE("Detected key action, emitting event: {}", event);
        EventProxy::get().emit<KeyEvent>(event);
    });

    m_window->onScrollCallback([](float offset) {
        ScrollEvent event{ offset };
        LOG_TRACE("Detected scroll, emitting event: {}", event);
        EventProxy::get().emit<ScrollEvent>(event);
    });

    m_window->onMouseCallback([](MouseAction action, int button) {
        MouseEvent event{ action, button };
        LOG_TRACE("Detected mouse action, emitting event: {}", event);
        EventProxy::get().emit<MouseEvent>(event);
    });

    m_window->onWindowResizeCallback([](uint32_t width, uint32_t height) {
        WindowResized event{ width, height };
        LOG_TRACE("Window resized, emitting event: {}", event);
        EventProxy::get().emit<WindowResized>(event);
    });
}

Vec2<u32> WindowManager::getSize() const { return m_window->getSize(); }

Vec2<u32> WindowManager::getFramebufferSize() const {
    return m_window->getFramebufferSize();
}

Vec2<f32> WindowManager::getMousePosition() const {
    return m_window->getMousePosition();
}

Vec2<f32> WindowManager::getMousePositionDelta() const {
    return m_mousePositionDelta;
}

void WindowManager::hideCursor() { m_window->hideCursor(); }

void WindowManager::showCursor() { m_window->showCursor(); }

void WindowManager::update() { calculateMousePositionDelta(); }

void WindowManager::calculateMousePositionDelta() {
    auto mousePosition      = m_window->getMousePosition();
    m_mousePositionDelta    = mousePosition - m_previousMousePosition;
    m_previousMousePosition = mousePosition;
}

}  // namespace sl
