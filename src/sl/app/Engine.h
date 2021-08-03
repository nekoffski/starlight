#pragma once

#include <memory>
#include <optional>

#include "sl/app/Application.h"
#include "sl/app/ConfigLoader.h"
#include "sl/core/Logger.h"
#include "sl/gui/fonts/FontAwesome.h"
#include "sl/platform/Platform.hpp"
#include "sl/utils/Globals.h"

#include <xvent/EventListener.h>

#include "sl/async/AsyncManager.hpp"
#include "sl/core/ClockManager.h"
#include "sl/core/Errors.hpp"
#include "sl/core/FileSystem.h"
#include "sl/core/Input.h"
#include "sl/core/InputManager.h"
#include "sl/core/Logger.h"
#include "sl/core/Profiler.h"
#include "sl/core/sig/Signal.h"
#include "sl/geom/GeometryManager.h"

#include "sl/gfx/fwd.h"

#include "sl/utils/Globals.h"

#include "sl/event/EventManager.h"

#include "sl/core/InputManager.h"
#include "sl/core/WindowManager.h"

#include "sl/platform/glfw/GlfwInput.h"

#include "sl/gui/GuiApi.h"

#include "sl/platform/gui/ImGuiApi.h"

namespace sl::app {

class Engine : xvent::EventListener {
public:
    class Builder {
    public:
        Builder&& setConfigFile(const std::string& path) &&;
        Builder&& setPlatform(platform::Platform* platform) &&;

        std::unique_ptr<Engine> build() &&;

    private:
        platform::Platform* m_platform;
        std::optional<std::string> m_configPath;
    };

    explicit Engine(const std::string& configPath, platform::Platform* platform);

    void handleEvents(const xvent::EventProvider&) override;

    ~Engine();

    void setApplication(std::unique_ptr<Application> application);
    void run();

private:
    void render();
    void update();
    void loopStep();

    platform::Platform* m_platform;

    core::Window* m_window;
    gfx::RenderApi* m_renderApi;
    core::Input* m_input;

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

    std::unique_ptr<gui::GuiApi> m_gui;
};

}