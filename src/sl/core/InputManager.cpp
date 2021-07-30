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

void InputManager::setMouse(Mouse* mouse) {
    m_mouse = mouse;

    m_mouse->setOnMousePositionChange(onMousePositionChange);
    m_mouse->setOnMouseScroll(onMouseScroll);
}

void InputManager::setKeyboard(Keyboard* keyboard) {
    m_keyboard = keyboard;
}

void InputManager::update() {
    static float previousPositionX = 0.0f;
    static float previousPositionY = 0.0f;

    auto mousePosition = getMousePosition();

    mousePositionDeltaX = mousePosition.x - previousPositionX;
    mousePositionDeltaY = mousePosition.y - previousPositionY;

    previousPositionX = mouseX;
    previousPositionY = mouseY;

    static float previousOffsetY = 0.0f;

    mouseScrollDelta = mouseScrollOffset - previousOffsetY;
    previousOffsetY = mouseScrollOffset;
}

bool InputManager::isMouseButtonPressed(int buttonCode) const {
    return m_mouse->isMouseButtonPressed(buttonCode) && not GLOBALS().flags.disableMouseInput;
}

math::Vec2 InputManager::getMousePosition() const {
    return m_mouse->getMousePosition();
}

math::Vec2 InputManager::getMousePositonDelta() const {
    return { mousePositionDeltaX, mousePositionDeltaY };
}

double InputManager::getScrollDelta() const {
    return mouseScrollDelta * not GLOBALS().flags.disableMouseInput;
}

bool InputManager::isKeyPressed(int keyCode) const {
    return m_keyboard->isKeyPressed(keyCode) && not GLOBALS().flags.disableKeyboardInput;
}

}