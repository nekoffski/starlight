#pragma once

#include <kc/core/Log.h>
#include <kc/event/EventListener.h>
#include <kc/sig/SignalHandler.h>

#include <memory>
#include <optional>

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
#include "sl/gpu/BufferManager.h"
#include "sl/gpu/ShaderManager.h"
#include "sl/gpu/TextureManager.h"
#include "sl/gpu/fwd.h"
#include "sl/glob/Globals.h"
#include "sl/gui/GuiHelper.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/physx/PhysicsEngine.h"
#include "sl/platform/Platform.hpp"
#include "sl/platform/glfw/GlfwInput.h"

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
        cfg::Config* m_config          = nullptr;
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
    gpu::RenderApi* m_renderApi;

    cfg::Config* m_config;

    std::unique_ptr<gpu::Renderer> m_renderer;
    Application* m_application;

    // clang-format off
    core::InputManager    m_inputManager;
    core::WindowManager   m_windowManager;
    async::AsyncManager   m_asyncManager;
    core::ClockManager    m_clockManager;
    gpu::ShaderManager    m_shaderManager;
    gpu::BufferManager    m_bufferManager;
    gpu::TextureManager   m_textureManager;
    event::EventManager   m_eventManager;
    geom::GeometryManager m_geometryManager;
    glob::Globals         m_globals;
    gui::GuiHelper        m_guiHelper;
    physx::PhysicsEngine  m_physxEngine;
    // clang-format on
};

}  // namespace sl::app