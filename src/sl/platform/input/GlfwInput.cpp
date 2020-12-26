#include "GlfwInput.h"

namespace sl::platform::input {

GlfwInput::GlfwInput(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {
}

void GlfwInput::setKeyCallback(core::InputCallback callback) {
    //glfwSetKeyCallback(m_windowHandle, );
}

bool GlfwInput::isKeyPressed(int keycode) const {
    return glfwGetKey(m_windowHandle, keycode);
}

bool GlfwInput::isMouseButtonPressed(int button) const {
    return glfwGetMouseButton(m_windowHandle, button);
}

std::pair<double, double> GlfwInput::getMousePosition() const {
    std::pair<double, double> pos;
    glfwGetCursorPos(m_windowHandle, &pos.first, &pos.second);
    return pos;
}
}
