#include "GlfwWindow.h"

#include "starlight/platform/vulkan/Vulkan.h"
#include <GLFW/glfw3.h>
#include <kc/core/Log.h>

#include "starlight/event/Event.h"
#include "starlight/event/Quit.h"
#include "starlight/core/Core.hpp"

namespace sl::platform::glfw {

#define GLFW_WINDOW_PTR(ptr) static_cast<GLFWwindow*>(ptr)
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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_windowHandle = glfwCreateWindow(1600, 900, "starlight-engine", nullptr, nullptr);

    glfwMakeContextCurrent(GLFW_WINDOW_PTR(m_windowHandle));
    glfwSetWindowUserPointer(GLFW_WINDOW_PTR(m_windowHandle), &m_callbacks);
}

bool GlfwWindow::isKeyPressed(core::Window::Key keyCode) const {
    return glfwGetKey(GLFW_WINDOW_PTR(m_windowHandle), keyCode) == GLFW_PRESS;
}

void GlfwWindow::onKeyCallback(OnKeyCallback callback) {
    m_callbacks.onKey = callback;

    static auto onKeyCallback =
        [](GLFWwindow* window, int key, [[maybe_unused]] int, int action, [[maybe_unused]] int) {
        GET_USER_CALLBACKS(window)->onKey(glfwToNovaKeyAction(action), key);
    };

    glfwSetKeyCallback(GLFW_WINDOW_PTR(m_windowHandle), onKeyCallback);
}

void GlfwWindow::onMouseCallback(OnMouseCallback callback) {
    m_callbacks.onMouse = callback;

    static auto onMouseButtonCallback =
        [](GLFWwindow* window, int button, int action, [[maybe_unused]] int) {
        GET_USER_CALLBACKS(window)->onMouse(glfwToNovaMouseAction(action), button);
    };

    glfwSetMouseButtonCallback(GLFW_WINDOW_PTR(m_windowHandle), onMouseButtonCallback);
}

void GlfwWindow::onWindowCloseCallback(OnWindowCloseCallback callback) {
    m_callbacks.onWindowClose = callback;

    static auto onWindowCloseCallback = [](GLFWwindow* window) {
        GET_USER_CALLBACKS(window)->onWindowClose();
    };

    glfwSetWindowCloseCallback(GLFW_WINDOW_PTR(m_windowHandle), onWindowCloseCallback);
}

void GlfwWindow::onWindowResizeCallback(OnWindowResizeCallback callback) {
    m_callbacks.onWindowResize = callback;

    static auto onWindowResizeCallback = [](GLFWwindow* window, int width, int height) {
        GET_USER_CALLBACKS(window)->onWindowResize(
            static_cast<uint32_t>(width), static_cast<uint32_t>(height)
        );
    };

    glfwSetWindowSizeCallback(GLFW_WINDOW_PTR(m_windowHandle), onWindowResizeCallback);
}

void GlfwWindow::update() { glfwPollEvents(); }

void GlfwWindow::swapBuffers() { glfwSwapBuffers(GLFW_WINDOW_PTR(m_windowHandle)); }

std::string_view GlfwWindow::getVendor() const { return "GLFW3"; }

math::Size2u32 GlfwWindow::getSize() const {
    int width, height;
    glfwGetWindowSize(GLFW_WINDOW_PTR(m_windowHandle), &width, &height);

    return math::Size2u32{static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

math::Vec2f GlfwWindow::getMousePosition() const {
    double x;
    double y;

    glfwGetCursorPos(GLFW_WINDOW_PTR(m_windowHandle), &x, &y);

    return math::Vec2f{x, y};
}

void* GlfwWindow::getHandle() { return m_windowHandle; }

}  // namespace sl::platform::glfw