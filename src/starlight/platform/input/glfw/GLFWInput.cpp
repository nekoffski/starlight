#include <starlight/platform/input/glfw/GLFWInput.h>

namespace starl::platform::input::glfw {

GLFWInput::GLFWInput(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {
}

void GLFWInput::setKeyCallback(InputCallback callback) {
    //glfwSetKeyCallback(m_windowHandle, );
}

bool GLFWInput::isKeyPressed(int keycode) const {
    return glfwGetKey(m_windowHandle, keycode);
}

bool GLFWInput::isMouseButtonPressed(int button) const {
    return glfwGetMouseButton(m_windowHandle, button);
}

std::pair<double, double> GLFWInput::getMousePosition() const {
    std::pair<double, double> pos;
    glfwGetCursorPos(m_windowHandle, &pos.first, &pos.second);
    return std::move(pos);
}
}
