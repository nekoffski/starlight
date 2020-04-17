#pragma once

#include <iostream>

#include <starlight/application/context/ApplicationContext.h>
#include <starlight/application/context/ApplicationContextResource.h>
#include <starlight/asset/AssetManager.hpp>
#include <starlight/framework/graphics/LowLevelRenderer.h>
#include <starlight/platform/input/Input.h>
#include <starlight/platform/window/Window.h>
#include <starlight/rendering/renderer/CubemapRenderer.h>
#include <starlight/rendering/renderer/ModelRenderer.h>

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
        return context::ApplicationContextResource(m_cubemapRenderer, m_modelRenderer, m_assetManager);
    }

    virtual void onStart() {}
    virtual void onStop() {}

private:
    std::unique_ptr<platform::window::Window> m_window;
    std::unique_ptr<platform::input::Input> m_input;
    std::unique_ptr<framework::graphics::LowLevelRenderer> m_lowLevelRenderer;
    std::shared_ptr<context::ApplicationContext> m_context;

    asset::AssetManager m_assetManager;
    std::shared_ptr<rendering::renderer::CubemapRenderer> m_cubemapRenderer;
    std::shared_ptr<rendering::renderer::ModelRenderer> m_modelRenderer;
};

class Application::Token {
    template <typename T>
    friend class Entrypoint;
    Token() = default;
};
}