#pragma once

#include <memory>

#include "sl/gui/GUIProxy.h"
#include "sl/scene/SceneManager.h"

namespace sl::platform::input {
class Input;
}

namespace sl::application {
class Application;
}

namespace sl::application::context {

class ApplicationContext {
    friend class Application;

public:
    virtual void onInit() = 0;
    virtual void onAttach() = 0;
    virtual void onDetach() = 0;

    virtual void handleInput(std::shared_ptr<platform::input::Input>) = 0;
    virtual void update(float, float) = 0;
    virtual void render() = 0;

    virtual void renderGUI(std::shared_ptr<gui::GUIProxy>) {}

    ApplicationContext& setSceneManager(std::shared_ptr<scene::SceneManager> sceneManager) {
        m_sceneManager = sceneManager;
        return *this;
    }

    ApplicationContext& setViewport(platform::window::Viewport viewport) {
        m_viewport = std::move(viewport);
        return *this;
    }

protected:
    std::shared_ptr<scene::SceneManager> m_sceneManager;
    platform::window::Viewport m_viewport;
};
}