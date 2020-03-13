#include <starlight/platform/window/glfw/GLFWWindow.h>

namespace starl::platform::window::glfw {

GLFWWindow::~GLFWWindow() {
    if (m_windowHandle != nullptr) {
        glfwDestroyWindow(m_windowHandle);
    }
}

void GLFWWindow::init() {
    if (auto status = glfwInit(); status < 0) {
        throw PlatformException(ErrorCode::COULD_NOT_INIT_WINDOW_LIBRARY, "Could not load GLFW", status);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_windowHandle = glfwCreateWindow(m_windowParams.viewport.width, m_windowParams.viewport.height,
        m_windowParams.title.c_str(), nullptr, nullptr);

    if (m_windowHandle == nullptr) {
        throw PlatformException(ErrorCode::COULD_NOT_CREATE_WINDOW_INSTANCE, "Could not create GLFW window instance");
    }
}

bool GLFWWindow::getShouldClose() const {
    return glfwWindowShouldClose(m_windowHandle);
}

void GLFWWindow::setShouldClose(bool value) {
    glfwSetWindowShouldClose(m_windowHandle, value);
}

void GLFWWindow::update(float dtime) {
    glfwPollEvents();
}
}