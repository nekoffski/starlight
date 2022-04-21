#include "InputManager.h"

#include "WindowManager.h"
#include "sl/glob/Globals.h"

namespace sl::core {

static double mousePositionDeltaX = 0.0f;
static double mousePositionDeltaY = 0.0f;
static double mouseScrollDelta = 0.0f;

static double mouseX = 0.0f;
static double mouseY = 0.0f;
static double mouseScrollOffset = 0.0f;

static void onMousePositionChange([[maybe_unused]] void* window, double positionX,
                                  double positionY) {
    mouseX = positionX;
    mouseY = positionY;
}

static void onMouseScroll([[maybe_unused]] void* window, [[maybe_unused]] double offsetX,
                          double offsetY) {
    const float speed = 0.1f;
    mouseScrollOffset -= offsetY * speed;
}

void InputManager::setMouse(Mouse* mouse) {
    m_mouse = mouse;

    m_mouse->setOnMousePositionChange(onMousePositionChange);
    m_mouse->setOnMouseScroll(onMouseScroll);
}

void InputManager::setKeyboard(Keyboard* keyboard) { m_keyboard = keyboard; }

void InputManager::update() {
    static float previousPositionX = 0.0f;
    static float previousPositionY = 0.0f;

    auto mousePosition = m_mouse->getMousePosition();

    mousePositionDeltaX = mousePosition.x - previousPositionX;
    mousePositionDeltaY = mousePosition.y - previousPositionY;

    previousPositionX = mouseX;
    previousPositionY = mouseY;

    static float previousOffsetY = 0.0f;

    mouseScrollDelta = mouseScrollOffset - previousOffsetY;
    previousOffsetY = mouseScrollOffset;
}

bool InputManager::isMouseButtonPressed(int buttonCode) const {
    return m_mouse->isMouseButtonPressed(buttonCode) &&
           not glob::Globals::get().flags.disableMouseInput;
}

math::Vec2 InputManager::getMousePosition() const {
    const auto yOffset =
        WindowManager::get().getSize().height - (m_viewport.height + m_viewport.beginY);
    const auto windowMousePosition = m_mouse->getMousePosition();

    return math::Vec2{windowMousePosition.x - m_viewport.beginX, windowMousePosition.y - yOffset};
}

math::Vec2 InputManager::getMousePositonDelta() const {
    return {mousePositionDeltaX, mousePositionDeltaY};
}

double InputManager::getScrollDelta() const {
    return mouseScrollDelta * not glob::Globals::get().flags.disableMouseInput;
}

bool InputManager::isKeyPressed(int keyCode) const {
    return m_keyboard->isKeyPressed(keyCode) && not glob::Globals::get().flags.disableKeyboardInput;
}

}  // namespace sl::core