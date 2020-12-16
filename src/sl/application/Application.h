#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>

#include "sl/application/ApplicationContext.h"
#include "sl/event/Event.h"
#include "sl/event/EventObserver.h"
#include "sl/gui/GuiProxy.h"
#include "sl/platform/gpu/GraphicsContext.h"
#include "sl/platform/gui/fwd.h"
#include "sl/platform/input/Input.h"
#include "sl/platform/window/Window.h"
#include "sl/graphics/Renderer.h"
#include "sl/scene/SceneSystems.hpp"

namespace sl::application {

class Entrypoint;

class Application : public event::EventObserver {
    class Token;

public:
    explicit Application(Token&&);

    void forceStop();
    bool isRunning() const;

    void update(float, float);
    void handleInput();
    void render();
    void renderGui();

    std::shared_ptr<ApplicationContext> getActiveContext() const;
    void handleEvents(event::EventPool& eventPool);
    void switchContext(std::shared_ptr<ApplicationContext> context);

    void init();

    // for custom user initialization
    virtual void onStart() {}
    virtual void onStop() {}

protected:
    template <typename T>
    requires std::derived_from<T, ApplicationContext>
        std::shared_ptr<T> createContext() {
        auto context = std::make_shared<T>();
        context->onInit();

        return std::dynamic_pointer_cast<T>(context);
    }

private:
    std::shared_ptr<platform::window::Window> m_window;
    std::shared_ptr<platform::input::Input> m_input;
    std::shared_ptr<graphics::Renderer> m_renderer;
    std::shared_ptr<platform::gui::GuiImpl> m_guiImpl;
    std::shared_ptr<platform::gpu::GraphicsContext> m_graphicsContext;
    std::shared_ptr<ApplicationContext> m_context;
    std::shared_ptr<gui::GuiProxy> m_guiProxy;
    std::shared_ptr<scene::SceneSystems> m_sceneSystems;
};

class Application::Token {
    friend class Entrypoint;
    Token() = default;
};
}

#define SL_APPLICATION using sl::application::Application::Application;
