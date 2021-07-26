#include "GlfwHud.h"

#include <GLFW/glfw3.h>

#include "sl/utils/Globals.h"

namespace sl::platform::core {

namespace {
    // TODO: investigate if this could be done in other way
    static double mousePositionDeltaX = 0.0f;
    static double mousePositionDeltaY = 0.0f;
    static double mouseScrollDelta = 0.0f;

    static double mouseX = 0.0f;
    static double mouseY = 0.0f;
    static double mouseScrollOffset = 0.0f;

    static void onMousePositionChange([[maybe_unused]] GLFWwindow* window, double positionX, double positionY) {
        mouseX = positionX;
        mouseY = positionY;
    }

    static void onMouseScroll([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double offsetX, double offsetY) {
        const float speed = 0.1f;
        mouseScrollOffset -= offsetY * speed;
    }
}

GlfwHud::GlfwHud(void* windowHandle)
    : m_windowHandle(windowHandle) {

    GLFWwindow* windowHandle_ = static_cast<GLFWwindow*>(m_windowHandle);

    glfwSetScrollCallback(windowHandle_, onMouseScroll);
    glfwSetCursorPosCallback(windowHandle_, onMousePositionChange);
}

bool GlfwHud::isKeyPressed(int keycode) const {
    return glfwGetKey(static_cast<GLFWwindow*>(m_windowHandle), keycode) && not GLOBALS().flags.disableKeyboardInput;
}

bool GlfwHud::isMouseButtonPressed(int button) const {
    return glfwGetMouseButton(static_cast<GLFWwindow*>(m_windowHandle), button) && not GLOBALS().flags.disableMouseInput;
}

std::pair<double, double> GlfwHud::getMousePosition() const {
    std::pair<double, double> pos;
    glfwGetCursorPos(static_cast<GLFWwindow*>(m_windowHandle), &pos.first, &pos.second);
    return pos;
}

void GlfwHud::setOnMousePositionChange(sl::core::Mouse::Callback callback) {
    glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_windowHandle), (GLFWcursorposfun)callback);
}

void GlfwHud::setOnMouseScroll(sl::core::Mouse::Callback callback) {
    glfwSetScrollCallback(static_cast<GLFWwindow*>(m_windowHandle), (GLFWscrollfun)callback);
}

}
