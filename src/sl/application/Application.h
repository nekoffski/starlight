#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>

#include "sl/application/context/ApplicationContext.h"
#include "sl/gui/GUIProxy.h"
#include "sl/platform/gui/fwd.h"
#include "sl/platform/input/Input.h"
#include "sl/platform/window/Window.h"
#include "sl/rendering/Renderer.h"
#include "sl/scene/SceneManager3D.h"
#include "sl/scene/SceneManager2D.h"

namespace sl::application {

class Application;

template <typename T>
concept Applicable = std::derived_from<T, Application>;

template <Applicable App>
class Entrypoint;

class Application {
    class Token;

public:
    explicit Application(Token&&);

    void forceStop();
    bool isRunning() const;

    void update(float, float);
    void handleInput();
    void render();
    void renderGUI();

    std::shared_ptr<context::ApplicationContext> getActiveContext() const {
        return m_context;
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
	template <typename T> requires std::derived_from<T, context::ApplicationContext>
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
    std::shared_ptr<platform::gui::GUIImpl> m_guiImpl;
    std::shared_ptr<context::ApplicationContext> m_context;
    std::shared_ptr<gui::GUIProxy> m_guiProxy;

    std::shared_ptr<scene::SceneManager> m_sceneManager3D;
    std::shared_ptr<scene::SceneManager> m_sceneManager2D;
};

class Application::Token {
    template <Applicable App>
    friend class Entrypoint;
    Token() = default;
};
}
