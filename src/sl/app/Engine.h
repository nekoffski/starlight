#pragma once

#include <memory>
#include <optional>

#include <kc/core/Log.h>
#include <kc/event/EventListener.h>
#include <kc/sig/SignalHandler.h>

#include "sl/app/Application.h"
#include "sl/async/AsyncManager.hpp"
#include "sl/cfg/Config.h"
#include "sl/core/ClockManager.h"
#include "sl/core/Errors.hpp"
#include "sl/core/Input.h"
#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"
#include "sl/event/EventManager.h"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/fwd.h"
#include "sl/glob/Globals.h"

#include "sl/gui/fonts/FontAwesome.h"
#include "sl/platform/Platform.hpp"
#include "sl/platform/glfw/GlfwInput.h"

#include "sl/async/AsyncManager.hpp"
#include "sl/core/ClockManager.h"
#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/BufferManager.h"
#include "sl/gfx/ShaderManager.h"
#include "sl/gfx/TextureManager.h"
#include "sl/gui/GuiHelper.h"

namespace sl::app {

class Engine : kc::event::EventListener {
public:
    class Builder {
    public:
        Builder&& setConfig(cfg::Config* config) &&;
        Builder&& setPlatform(platform::Platform* platform) &&;

        std::unique_ptr<Engine> build() &&;

    private:
        platform::Platform* m_platform = nullptr;
        cfg::Config* m_config = nullptr;
    };

    explicit Engine(cfg::Config* config, platform::Platform* platform);
    ~Engine();

    void handleEvents(const kc::event::EventProvider&) override;

    void setApplication(Application* application);
    void initGlobalState();
    void run();

private:
    void render();
    void update();
    void loopStep();

    void initManagers();
    void initLowLevelComponents();

    platform::Platform* m_platform;

    core::Window* m_window;
    core::Input* m_input;
    gfx::RenderApi* m_renderApi;

    cfg::Config* m_config;

    std::unique_ptr<gfx::Renderer> m_renderer;
    Application* m_application;

    // clang-format off
    core::InputManager    m_inputManager;
    core::WindowManager   m_windowManager;
    async::AsyncManager   m_asyncManager;
    core::ClockManager    m_clockManager;
    gfx::ShaderManager    m_shaderManager;
    gfx::BufferManager    m_bufferManager;
    gfx::TextureManager   m_textureManager;
    event::EventManager   m_eventManager;
    geom::GeometryManager m_geometryManager;
    glob::Globals         m_globals;
    gui::GuiHelper        m_guiHelper;
    // clang-format on
};

}