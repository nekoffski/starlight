#include "GlfwInput.h"

#include <GLFW/glfw3.h>

namespace sl::platform::glfw {

void GlfwInput::init(void* windowHandle) {
    m_windowHandle = windowHandle;
}

bool GlfwInput::isKeyPressed(int keycode) const {
    return glfwGetKey(static_cast<GLFWwindow*>(m_windowHandle), keycode);
}

bool GlfwInput::isMouseButtonPressed(int button) const {
    return glfwGetMouseButton(static_cast<GLFWwindow*>(m_windowHandle), button);
}

math::Vec2 GlfwInput::getMousePosition() const {
    double x, y;
    glfwGetCursorPos(static_cast<GLFWwindow*>(m_windowHandle), &x, &y);
    return { x, y };
}

void GlfwInput::setOnMousePositionChange(sl::core::Mouse::Callback callback) {
    glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_windowHandle), (GLFWcursorposfun)callback);
}

void GlfwInput::setOnMouseScroll(sl::core::Mouse::Callback callback) {
    glfwSetScrollCallback(static_cast<GLFWwindow*>(m_windowHandle), (GLFWscrollfun)callback);
}

}
