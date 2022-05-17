#include "GlfwWindow.h"

#include <GLFW/glfw3.h>
#include <kc/core/Log.h>

#include "nova/event/Event.h"
#include "nova/event/Quit.h"
#include "nova/event/Key.h"

namespace nova::platform::glfw {

#define TO_GLFW_PTR(ptr) static_cast<GLFWwindow*>(ptr)

using namespace event;

static KeyEvent::Action glfwToNovaAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return KeyEvent::Action::press;

        case GLFW_REPEAT:
            return KeyEvent::Action::repeat;

        case GLFW_RELEASE:
            return KeyEvent::Action::release;
    }

    return KeyEvent::Action::unknown;
}

GlfwWindow::GlfwWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_windowHandle = glfwCreateWindow(1600, 900, "nova-engine", nullptr, nullptr);

    glfwMakeContextCurrent(TO_GLFW_PTR(m_windowHandle));

    static auto onWindowClose = []([[maybe_unused]] GLFWwindow*) -> void {
        LOG_INFO("Window received close request");
        EventManager::get().emitEvent<QuitEvent>("Quit button pressed");
    };

    glfwSetWindowCloseCallback(TO_GLFW_PTR(m_windowHandle), onWindowClose);

    static auto onKeyEvent = []([[maybe_unused]] GLFWwindow*, int key,
                                [[maybe_unused]] int scancode, int action,
                                [[maybe_unused]] int mods) -> void {
        KeyEvent event{.action = glfwToNovaAction(action), .key = key};
        LOG_TRACE("Emitting event: {}", event);
        EventManager::get().emitEvent<KeyEvent>(event);
    };

    glfwSetKeyCallback(TO_GLFW_PTR(m_windowHandle), onKeyEvent);
}

void GlfwWindow::update() { glfwPollEvents(); }

void GlfwWindow::swapBuffers() { glfwSwapBuffers(TO_GLFW_PTR(m_windowHandle)); }

std::string_view GlfwWindow::getVendor() const { return "GLFW3"; }

math::Size2i GlfwWindow::getSize() const {
    math::Size2i size;
    glfwGetWindowSize(TO_GLFW_PTR(m_windowHandle), &size.width, &size.height);

    return size;
}

}  // namespace nova::platform::glfw