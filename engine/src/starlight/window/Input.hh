#pragma once

#include "kstd/Singleton.hh"
#include "Window.hh"

namespace sl {

class Input : public kstd::Singleton<Input> {
public:
    enum class State : u8 { on, off };

    using Button = Window::Button;
    using Key    = Window::Key;

    explicit Input(Window::Impl& window);

    Vec2<f32> getMousePosition() const;
    Vec2<f32> getMousePositionDelta() const;

    void showCursor();
    void hideCursor();

    bool isKeyPressed(Key keyCode) const;
    bool isMouseButtonPressed(Button buttonCode) const;

    void update();

    void switchKeyboardInput(State state);
    void switchMouseInput(State state);

private:
    void setCallbacks();
    void calculateMousePositionDelta();

    State m_keyboardState;
    State m_mouseState;

    Window::Impl& m_window;

    Vec2<f32> m_previousMousePosition;
    Vec2<f32> m_mousePositionDelta;
};

}  // namespace sl
