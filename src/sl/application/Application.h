#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>

#include "sl/application/context/ApplicationContext.h"
#include "sl/event/Event.h"
#include "sl/event/EventObserver.h"
#include "sl/gui/GuiProxy.h"
#include "sl/platform/gpu/GraphicsContext.h"
#include "sl/platform/gui/fwd.h"
#include "sl/platform/input/Input.h"
#include "sl/platform/window/Window.h"
#include "sl/rendering/Renderer.h"
#include "sl/scene/SceneManager2D.h"
#include "sl/scene/SceneManager3D.h"

namespace sl::application {

class Application;

/*template <typename T>*/
//concept Applicable = std::derived_from<T, Application>;

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

    std::shared_ptr<context::ApplicationContext> getActiveContext() const {
        return m_context;
    }

    void handleEvents(event::EventPool& eventPool) {
        for (auto& event : eventPool.getEventsByCategory({ event::EventCategory::CORE })) {
            switch (event->getType()) {
            case event::EventType::WINDOW_RESIZED: {

                auto windowResizeEvent = event->as<event::WindowResizedEvent>();
                SL_INFO("{}/{}", windowResizeEvent->width, windowResizeEvent->height);
                m_renderer->setTemporaryViewport(windowResizeEvent->width, windowResizeEvent->height); // TODO: whhy temporary?
                break;
            }
            }
        }
    }

    void switchContext(std::shared_ptr<context::ApplicationContext> context) {
        if (m_context)
            m_context->onDetach();
        m_context = context;
        m_context->onAttach();
    }

    void init();

    virtual void onStart() {}
    virtual void onStop() {}
    virtual void preInit() {}

protected:
    template <typename T>
    requires std::derived_from<T, context::ApplicationContext>
        std::shared_ptr<T> createContext(context::ContextType contextType) {
        auto context = std::make_shared<T>();

        context->setViewport(m_window->getParams().viewport);
        context->setSceneManager(contextType == context::CONTEXT_2D ? m_sceneManager2D : m_sceneManager3D);
        context->setWindow(m_window);
        context->onInit();

        return std::dynamic_pointer_cast<T>(context);
    }

private:
    std::shared_ptr<platform::window::Window> m_window;
    std::shared_ptr<platform::input::Input> m_input;
    std::shared_ptr<rendering::Renderer> m_renderer;
    std::shared_ptr<platform::gui::GuiImpl> m_guiImpl;
	std::shared_ptr<platform::gpu::GraphicsContext> m_graphicsContext;
	std::shared_ptr<context::ApplicationContext> m_context;
    std::shared_ptr<gui::GuiProxy> m_guiProxy;

    std::shared_ptr<scene::SceneManager> m_sceneManager3D;
    std::shared_ptr<scene::SceneManager> m_sceneManager2D;
};

class Application::Token {
    friend class Entrypoint;
    Token() = default;
};
}
