#include "GlfwInput.h"

#include <GLFW/glfw3.h>

#include "sl/utils/Globals.h"

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
    math::Vec2 position;
    glfwGetCursorPos(static_cast<GLFWwindow*>(m_windowHandle), (double*)&position.x, (double*)&position.y);
    return position;
}

void GlfwInput::setOnMousePositionChange(sl::core::Mouse::Callback callback) {
    glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_windowHandle), (GLFWcursorposfun)callback);
}

void GlfwInput::setOnMouseScroll(sl::core::Mouse::Callback callback) {
    glfwSetScrollCallback(static_cast<GLFWwindow*>(m_windowHandle), (GLFWscrollfun)callback);
}

}
