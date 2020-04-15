#pragma once

#include <iostream>

#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/input/Input.h>
#include <starlight/platform/window/Window.h>

namespace starl::application {

template <typename T>
class Entrypoint;

class Application {
    class Token;

public:
    explicit Application(Token&&);

    void forceStop();
    bool isRunning() const;

    void update(float);
    void handleInput();
    void render();

    virtual void onStart() {}
    virtual void onStop() {}

private:
    std::unique_ptr<platform::window::Window> m_window;
    std::unique_ptr<platform::input::Input> m_input;
    std::unique_ptr<framework::graphics::LowLevelRenderer> m_lowLevelRenderer;
};

class Application::Token {
    template <typename T>
    friend class Entrypoint;
    Token() = default;
};
}