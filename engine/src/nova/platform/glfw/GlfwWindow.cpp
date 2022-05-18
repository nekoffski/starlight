#include "GlfwWindow.h"

#include <GLFW/glfw3.h>
#include <kc/core/Log.h>

#include "nova/event/Event.h"
#include "nova/event/Quit.h"
#include "nova/core/Core.hpp"

namespace nova::platform::glfw {

#define TO_GLFW_PTR(ptr) static_cast<GLFWwindow*>(ptr)
#define GET_USER_CALLBACKS(window) static_cast<Callbacks*>(glfwGetWindowUserPointer(window))

using namespace event;

static core::KeyAction glfwToNovaKeyAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return core::KeyAction::press;

        case GLFW_REPEAT:
            return core::KeyAction::repeat;

        case GLFW_RELEASE:
            return core::KeyAction::release;
    }

    return core::KeyAction::unknown;
}

static core::MouseAction glfwToNovaMouseAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return core::MouseAction::press;

        case GLFW_RELEASE:
            return core::MouseAction::release;
    }

    return core::MouseAction::unknown;
}

GlfwWindow::GlfwWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_windowHandle = glfwCreateWindow(1600, 900, "nova-engine", nullptr, nullptr);

    glfwMakeContextCurrent(TO_GLFW_PTR(m_windowHandle));
    glfwSetWindowUserPointer(TO_GLFW_PTR(m_windowHandle), &m_callbacks);
}

void GlfwWindow::onKeyCallback(OnKeyCallback callback) {
    m_callbacks.onKey = callback;

    static auto onKeyCallback =
        [](GLFWwindow* window, int key, [[maybe_unused]] int, int action, [[maybe_unused]] int) {
        GET_USER_CALLBACKS(window)->onKey(glfwToNovaKeyAction(action), key);
    };

    glfwSetKeyCallback(TO_GLFW_PTR(m_windowHandle), onKeyCallback);
}

void GlfwWindow::onMouseCallback(OnMouseCallback callback) {
    m_callbacks.onMouse = callback;

    static auto onMouseButtonCallback =
        [](GLFWwindow* window, int button, int action, [[maybe_unused]] int) {
        GET_USER_CALLBACKS(window)->onMouse(glfwToNovaMouseAction(action), button);
    };

    glfwSetMouseButtonCallback(TO_GLFW_PTR(m_windowHandle), onMouseButtonCallback);
}

void GlfwWindow::onWindowCloseCallback(OnWindowCloseCallback callback) {
    m_callbacks.onWindowClose = callback;

    static auto onWindowCloseCallback = [](GLFWwindow* window) {
        GET_USER_CALLBACKS(window)->onWindowClose();
    };

    glfwSetWindowCloseCallback(TO_GLFW_PTR(m_windowHandle), onWindowCloseCallback);
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