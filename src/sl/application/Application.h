#pragma once

#include <iostream>

#include "sl/application/context/ApplicationContext.h"
#include "sl/gui/GUIProxy.h"
#include "sl/platform/gui/GUIAdapter.hpp"
#include "sl/platform/input/Input.h"
#include "sl/platform/window/Window.h"
#include "sl/rendering/Renderer.h"
#include "sl/scene/SceneManager.h"

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
    template <class T>
    std::shared_ptr<T> createContext() {
        auto context = std::make_shared<T>();
        // TO REDESIGN - pretty ugly
        std::static_pointer_cast<context::ApplicationContext>(context)
            ->setViewport(m_window->getParams().viewport)
            .setSceneManager(m_sceneManager);
        context->onInit();
        return context;
    }

private:
    std::shared_ptr<platform::window::Window> m_window;
    std::shared_ptr<platform::input::Input> m_input;
	std::shared_ptr<rendering::Renderer> m_renderer;
    std::shared_ptr<platform::gui::GUIAdapter> m_guiAdapter;
    std::shared_ptr<context::ApplicationContext> m_context;
    std::shared_ptr<gui::GUIProxy> m_guiProxy;
    std::shared_ptr<scene::SceneManager> m_sceneManager;
};

class Application::Token {
    template <Applicable App>
    friend class Entrypoint;
    Token() = default;
};
}
