#include "WindowManager.h"

#include <kc/core/Log.h>

#include "nova/event/Event.h"
#include "nova/event/Quit.h"
#include "nova/event/Input.h"

namespace nova::core {

WindowManager::WindowManager(Window* window) : m_window(window) { setCallbacks(); }

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

}  // namespace nova::core
