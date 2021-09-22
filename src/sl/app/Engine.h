#pragma once

#include <memory>
#include <optional>

#include <kc/core/Log.h>
#include <kc/event/EventListener.h>

#include "sl/app/Application.h"
#include "sl/app/ConfigLoader.h"
#include "sl/async/AsyncManager.hpp"
#include "sl/core/ClockManager.h"
#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Input.h"
#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"
#include "sl/core/sig/Signal.h"
#include "sl/event/EventManager.h"
#include "sl/geom/GeometryManager.h"
#include "sl/gfx/fwd.h"
#include "sl/gui/GuiApi.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/platform/Platform.hpp"
#include "sl/platform/glfw/GlfwInput.h"
#include "sl/platform/gui/ImGuiApi.h"
#include "sl/utils/Globals.h"

namespace sl::app {

class Engine : kc::event::EventListener {
public:
    class Builder {
    public:
        Builder&& setConfig(utils::Config* config) &&;
        Builder&& setPlatform(platform::Platform* platform) &&;

        std::unique_ptr<Engine> build() &&;

    private:
        platform::Platform* m_platform = nullptr;
        utils::Config* m_config = nullptr;
    };

    explicit Engine(utils::Config* config, platform::Platform* platform);
    ~Engine();

    void handleEvents(const kc::event::EventProvider&) override;

    void setApplication(std::unique_ptr<Application> application);
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

    utils::Config* m_config;

    std::unique_ptr<gfx::Renderer> m_renderer;
    std::unique_ptr<Application> m_application;

    std::unique_ptr<core::InputManager> m_inputManager;

    std::unique_ptr<core::WindowManager> m_windowManager;
    std::unique_ptr<async::AsyncManager> m_asyncManager;
    std::unique_ptr<core::ClockManager> m_clockManager;
    std::unique_ptr<gfx::ShaderManager> m_shaderManager;
    std::unique_ptr<gfx::BufferManager> m_bufferManager;
    std::unique_ptr<gfx::TextureManager> m_textureManager;
    std::unique_ptr<event::EventManager> m_eventManager;
    std::unique_ptr<geom::GeometryManager> m_geometryManager;
    std::unique_ptr<utils::Globals> m_globals;
};

}