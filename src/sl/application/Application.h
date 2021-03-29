#pragma once

#include <concepts>
#include <iostream>
#include <memory>
#include <type_traits>

#include <xvent/EventEmitter.h>
#include <xvent/EventEngine.h>
#include <xvent/EventListener.h>

#include "sl/application/ApplicationContext.h"
#include "sl/core/Input.h"
#include "sl/core/Window.h"
#include "sl/graphics/GraphicsContext.h"
#include "sl/graphics/LowLevelRenderer.h"
#include "sl/graphics/Renderer.hpp"
#include "sl/gui/GuiApi.h"
#include "sl/scene/SceneSystems.hpp"

namespace sl::application {

class Entrypoint;

class Application {
public:
    explicit Application();

    void forceStop();
    bool isRunning() const;

    void update(float, float);
    void handleInput();
    void render();
    void renderGui();

    std::shared_ptr<ApplicationContext> getActiveContext() const;

    void switchContext(std::shared_ptr<ApplicationContext> context);

    void init();

    // for custom user initialization
    virtual void onStart() { }
    virtual void onStop() { }

protected:
    // clang-format off
    template <typename T>
    requires std::derived_from<T, ApplicationContext> && std::constructible_from<T, const std::string&>
    std::shared_ptr<T> createContext(const std::string& ident) {
        auto context = std::make_shared<T>(ident);

        context->m_guiApiProxy = std::make_shared<gui::GuiApiProxy>(m_guiApi);
        context->m_lowLevelRendererProxy = std::make_shared<graphics::LowLevelRendererProxy>(m_lowLevelRenderer);
        context->m_windowProxy = std::make_shared<core::WindowProxy>(m_window);

        context->onInit();
        m_eventEngine.registerEventListener(context);

        return std::dynamic_pointer_cast<T>(context);
    }
    // clang-format on

private:
    std::shared_ptr<core::Window> m_window;
    std::shared_ptr<core::Input> m_input;
    std::shared_ptr<graphics::LowLevelRenderer> m_lowLevelRenderer;
    std::shared_ptr<gui::GuiApi> m_guiApi;
    std::shared_ptr<graphics::GraphicsContext> m_graphicsContext;
    std::shared_ptr<ApplicationContext> m_context;
    std::shared_ptr<scene::SceneSystems> m_sceneSystems;
    std::shared_ptr<graphics::Renderer> m_renderer;

    xvent::EventEngine m_eventEngine;
    std::shared_ptr<xvent::EventEmitter> m_eventEmitter;
};
}
