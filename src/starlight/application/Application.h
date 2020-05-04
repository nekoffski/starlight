#pragma once

#include <iostream>

#include <starlight/application/context/ApplicationContext.h>
#include <starlight/application/context/ApplicationContextResource.h>
#include <starlight/asset/AssetManager.hpp>
#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/gui/GUIProxy.h>
#include <starlight/platform/gui/GUIAdapter.hpp>
#include <starlight/platform/input/Input.h>
#include <starlight/platform/window/Window.h>
#include <starlight/rendering/renderer/CubemapRenderer.h>
#include <starlight/rendering/renderer/ModelRenderer.h>
#include <starlight/scene/SceneManager.h>

namespace starl::application {

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

    void update(float);
    void handleInput();
    void render();
    void renderGUI();

    const std::shared_ptr<context::ApplicationContext>& getContext() const {
        return m_context;
    }

    void switchContext(std::shared_ptr<context::ApplicationContext> context) {
        if (m_context)
            m_context->onDetach();
        m_context = std::move(context);
        m_context->onAttach();
    }

    context::ApplicationContextResource createApplicationContextResource() {
        return context::ApplicationContextResource(m_assetManager, m_sceneManager, m_window->getParams().viewport);
    }

    virtual void onStart() {}
    virtual void onStop() {}

private:
    std::unique_ptr<platform::window::Window> m_window;
    std::unique_ptr<platform::input::Input> m_input;
    std::unique_ptr<framework::graphics::LowLevelRenderer> m_lowLevelRenderer;
    std::shared_ptr<platform::gui::GUIAdapter> m_guiAdapter;
    std::shared_ptr<context::ApplicationContext> m_context;
    std::shared_ptr<gui::GUIProxy> m_guiProxy;

    asset::AssetManager m_assetManager;
    std::shared_ptr<rendering::renderer::CubemapRenderer> m_cubemapRenderer;
    std::shared_ptr<rendering::renderer::ModelRenderer> m_modelRenderer;
    std::shared_ptr<scene::SceneManager> m_sceneManager;
};

class Application::Token {
    template <Applicable App>
    friend class Entrypoint;
    Token() = default;
};
}