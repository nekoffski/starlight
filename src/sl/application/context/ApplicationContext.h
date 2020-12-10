#pragma once

#include <memory>

#include "sl/gui/GuiProxy.h"
#include "sl/platform/window/Window.h"
#include "sl/scene/SceneManager.h"
#include "sl/scene/SceneManager3D.h"

namespace sl::platform::input {
class Input;
}

namespace sl::application {
class Application;
}

namespace sl::application::context {

enum ContextType {
    CONTEXT_2D,
    CONTEXT_3D
};

class ApplicationContext {
    friend class Application;

public:
    virtual void onInit() = 0;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual void handleInput(std::shared_ptr<platform::input::Input>) = 0;
    virtual void update(float, float) = 0;
    virtual void render() = 0;

    virtual void renderGui(gui::GuiProxy&) {}

    void setSceneManager(std::shared_ptr<scene::SceneManager> sceneManager) {
        m_sceneManager = sceneManager;
    }

    void setWindow(std::shared_ptr<platform::window::Window> window) {
        m_window = window;
    }

    void setViewport(platform::window::Viewport viewport) {
        m_viewport = std::move(viewport);
    }

protected:
    std::shared_ptr<platform::window::Window> m_window;
    std::shared_ptr<scene::SceneManager> m_sceneManager;
    platform::window::Viewport m_viewport;
};
}
