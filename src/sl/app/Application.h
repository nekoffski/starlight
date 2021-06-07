#pragma once

#include <concepts>
#include <iostream>
#include <memory>
#include <type_traits>

#include <xvent/EventEmitter.h>
#include <xvent/EventEngine.h>
#include <xvent/EventListener.h>

#include "SceneSystems.h"
#include "fwd.h"
#include "sl/app/ApplicationContext.h"
#include "sl/core/Input.h"
#include "sl/core/Window.h"
#include "sl/gfx/GraphicsContext.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/Renderer.hpp"
#include "sl/gui/GuiApi.h"

namespace sl::app {

class Application {
public:
    explicit Application();

    static void initDefaultFactories();

    void forceStop();
    bool isRunning() const;

    void update(float, float);
    void render();

    std::shared_ptr<ApplicationContext> getActiveContext() const;

    void switchContext(std::shared_ptr<ApplicationContext> context);

    void init();

    // for custom user initialization
    virtual void onStart() {
    }

    virtual void onStop() {
    }

    // clang-format off
    template <typename T>
    requires std::derived_from<T, ApplicationContext> && std::constructible_from<T, const std::string&>
    std::shared_ptr<T> createContext(const std::string& ident) {
        auto context = std::make_shared<T>(ident);

        context->m_guiApiProxy = std::make_shared<gui::GuiApiProxy>(m_guiApi);
        context->m_lowLevelRendererProxy = std::make_shared<gfx::LowLevelRendererProxy>(m_lowLevelRenderer.get());
        context->m_windowProxy = std::make_shared<core::WindowProxy>(m_window);

        context->onInit();
        m_eventEngine.registerEventListener(context);

        return std::dynamic_pointer_cast<T>(context);
    }
    // clang-format on

private:
    void renderGui();

    std::shared_ptr<core::Window> m_window;
    std::shared_ptr<core::Input> m_input;
    std::unique_ptr<gfx::LowLevelRenderer> m_lowLevelRenderer;
    std::shared_ptr<gui::GuiApi> m_guiApi;
    std::shared_ptr<gfx::GraphicsContext> m_gfxContext;
    std::shared_ptr<ApplicationContext> m_context;
    std::unique_ptr<SceneSystems> m_sceneSystems;
    std::unique_ptr<gfx::Renderer> m_renderer;

    xvent::EventEngine m_eventEngine;
    std::shared_ptr<xvent::EventEmitter> m_eventEmitter;
};
}
