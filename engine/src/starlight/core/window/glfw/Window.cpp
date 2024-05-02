#include "Window.h"

#include <GLFW/glfw3.h>

#include "starlight/core/Log.h"
#include "starlight/core/event/Input.h"

namespace sl::glfw {

#define GLFW_WINDOW_PTR(ptr) static_cast<GLFWwindow*>(ptr)
#define GET_USER_CALLBACKS(window) \
    static_cast<Callbacks*>(glfwGetWindowUserPointer(window))

static KeyAction glfwToNovaKeyAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return KeyAction::press;

        case GLFW_REPEAT:
            return KeyAction::repeat;

        case GLFW_RELEASE:
            return KeyAction::release;
    }

    return KeyAction::unknown;
}

static MouseAction glfwToNovaMouseAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return MouseAction::press;

        case GLFW_RELEASE:
            return MouseAction::release;
    }

    return MouseAction::unknown;
}

Window::Window() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_windowHandle =
      glfwCreateWindow(1920, 1080, "starlight-engine", nullptr, nullptr);

    glfwMakeContextCurrent(GLFW_WINDOW_PTR(m_windowHandle));
    glfwSetWindowUserPointer(GLFW_WINDOW_PTR(m_windowHandle), &m_callbacks);
}

bool Window::isKeyPressed(Window::Key keyCode) const {
    return glfwGetKey(GLFW_WINDOW_PTR(m_windowHandle), keyCode) == GLFW_PRESS;
}

bool Window::isMouseButtonPressed(Window::Button buttonCode) const {
    return glfwGetMouseButton(GLFW_WINDOW_PTR(m_windowHandle), buttonCode)
           == GLFW_PRESS;
}

void Window::onKeyCallback(OnKeyCallback callback) {
    m_callbacks.onKey = callback;

    static auto onKeyCallback =
      [](
        GLFWwindow* window, int key, [[maybe_unused]] int, int action,
        [[maybe_unused]] int
      ) { GET_USER_CALLBACKS(window)->onKey(glfwToNovaKeyAction(action), key); };

    glfwSetKeyCallback(GLFW_WINDOW_PTR(m_windowHandle), onKeyCallback);
}

void Window::onMouseCallback(OnMouseCallback callback) {
    m_callbacks.onMouse = callback;

    static auto onMouseButtonCallback =
      [](GLFWwindow* window, int button, int action, [[maybe_unused]] int) {
          GET_USER_CALLBACKS(window)->onMouse(glfwToNovaMouseAction(action), button);
      };

    glfwSetMouseButtonCallback(
      GLFW_WINDOW_PTR(m_windowHandle), onMouseButtonCallback
    );
}

void Window::onScrollCallback(OnScrollCallback callback) {
    m_callbacks.onScroll = callback;

    static auto onScrollCallback =
      [](GLFWwindow* window, [[maybe_unused]] double xOffset, double yOffset) {
          GET_USER_CALLBACKS(window)->onScroll(static_cast<float>(yOffset));
      };

    glfwSetScrollCallback(GLFW_WINDOW_PTR(m_windowHandle), onScrollCallback);
}

void Window::onWindowCloseCallback(OnWindowCloseCallback callback) {
    m_callbacks.onWindowClose = callback;

    static auto onWindowCloseCallback = [](GLFWwindow* window) {
        GET_USER_CALLBACKS(window)->onWindowClose();
    };

    glfwSetWindowCloseCallback(
      GLFW_WINDOW_PTR(m_windowHandle), onWindowCloseCallback
    );
}

void Window::onWindowResizeCallback(OnWindowResizeCallback callback) {
    m_callbacks.onWindowResize = callback;

    static auto onWindowResizeCallback =
      [](GLFWwindow* window, int width, int height) {
          GET_USER_CALLBACKS(window)->onWindowResize(
            static_cast<float>(width), static_cast<float>(height)
          );
      };

    glfwSetWindowSizeCallback(
      GLFW_WINDOW_PTR(m_windowHandle), onWindowResizeCallback
    );
}

void Window::update() { glfwPollEvents(); }

void Window::hideCursor() {
    glfwSetInputMode(
      GLFW_WINDOW_PTR(m_windowHandle), GLFW_CURSOR, GLFW_CURSOR_DISABLED
    );
}

void Window::showCursor() {
    glfwSetInputMode(
      GLFW_WINDOW_PTR(m_windowHandle), GLFW_CURSOR, GLFW_CURSOR_NORMAL
    );
}

void Window::swapBuffers() { glfwSwapBuffers(GLFW_WINDOW_PTR(m_windowHandle)); }

std::string_view Window::getVendor() const { return "GLFW3"; }

Vec2u32 Window::getFramebufferSize() const {
    int width, height;
    glfwGetFramebufferSize(GLFW_WINDOW_PTR(m_windowHandle), &width, &height);

    return Vec2u32{ static_cast<u32>(width), static_cast<u32>(height) };
}

Vec2u32 Window::getSize() const {
    int width, height;
    glfwGetWindowSize(GLFW_WINDOW_PTR(m_windowHandle), &width, &height);

    return Vec2u32{ static_cast<u32>(width), static_cast<u32>(height) };
}

Vec2f Window::getMousePosition() const {
    double x, y;
    glfwGetCursorPos(GLFW_WINDOW_PTR(m_windowHandle), &x, &y);

    return Vec2f{ x, y };
}

void* Window::getHandle() { return m_windowHandle; }

}  // namespace sl::glfw