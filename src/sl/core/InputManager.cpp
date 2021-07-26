#include "InputManager.h"

#include "sl/utils/Globals.h"

namespace sl::core {

static double mousePositionDeltaX = 0.0f;
static double mousePositionDeltaY = 0.0f;
static double mouseScrollDelta = 0.0f;

static double mouseX = 0.0f;
static double mouseY = 0.0f;
static double mouseScrollOffset = 0.0f;

static void onMousePositionChange([[maybe_unused]] void* window, double positionX, double positionY) {
    mouseX = positionX;
    mouseY = positionY;
}

static void onMouseScroll([[maybe_unused]] void* window, [[maybe_unused]] double offsetX, double offsetY) {
    const float speed = 0.1f;
    mouseScrollOffset -= offsetY * speed;
}

// clang-format off
InputManager& InputManager::setMouse(Mouse* mouse) {
    m_mouse = mouse;

    m_mouse->setOnMousePositionChange(onMousePositionChange);
    m_mouse->setOnMouseScroll(onMouseScroll);

    return *this;
}

InputManager& InputManager::setKeyboard(Keyboard* keyboard) {
    m_keyboard = keyboard;
    return *this;
}
// clang-format on

void InputManager::update() {
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

bool InputManager::isMouseButtonPressed(int buttonCode) const {
    return m_mouse->isMouseButtonPressed(buttonCode);
}

std::pair<double, double> InputManager::getMousePosition() const {
    return m_mouse->getMousePosition();
}

std::pair<double, double> InputManager::getMousePositonDelta() const {
    return { mousePositionDeltaX, mousePositionDeltaY };
}

double InputManager::getScrollDelta() const {
    return mouseScrollDelta * not GLOBALS().flags.disableMouseInput;
}

bool InputManager::isKeyPressed(int keyCode) const {
    return m_keyboard->isKeyPressed(keyCode);
}

}