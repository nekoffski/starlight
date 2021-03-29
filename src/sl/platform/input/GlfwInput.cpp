#include "GlfwInput.h"

#include "sl/utils/Globals.h"

namespace sl::platform::input {

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

GlfwInput::GlfwInput(void* windowHandle)
    : m_windowHandle(static_cast<GLFWwindow*>(windowHandle)) {

    glfwSetScrollCallback(m_windowHandle, onMouseScroll);
    glfwSetCursorPosCallback(m_windowHandle, onMousePositionChange);
}

void GlfwInput::update() {
    static double previousPositionX = 0.0f;
    static double previousPositionY = 0.0f;

    auto [mouseX, mouseY] = getMousePosition();

    mousePositionDeltaX = mouseX - previousPositionX;
    mousePositionDeltaY = mouseY - previousPositionY;

    previousPositionX = mouseX;
    previousPositionY = mouseY;

    static double previousOffsetY = 0.0f;

    mouseScrollDelta = mouseScrollOffset - previousOffsetY;
    previousOffsetY = mouseScrollOffset;
}

void GlfwInput::setKeyCallback(core::InputCallback callback) {
    //glfwSetKeyCallback(m_windowHandle, );
}

bool GlfwInput::isKeyPressed(int keycode) const {
    return glfwGetKey(m_windowHandle, keycode) && not GLOBALS().flags.disableKeyboardInput;
}

bool GlfwInput::isMouseButtonPressed(int button) const {
    return glfwGetMouseButton(m_windowHandle, button) && not GLOBALS().flags.disableMouseInput;
}

std::pair<double, double> GlfwInput::getMousePosition() const {
    std::pair<double, double> pos;
    glfwGetCursorPos(m_windowHandle, &pos.first, &pos.second);
    return pos;
}

std::pair<double, double> GlfwInput::getMousePositonDelta() const {
    return { mousePositionDeltaX, mousePositionDeltaY };
}

double GlfwInput::getScrollDelta() const {
    return mouseScrollDelta * not GLOBALS().flags.disableMouseInput;
}
}
