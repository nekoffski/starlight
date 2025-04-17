#pragma once

#include <atomic>
#include <optional>

#include <starlight/core/math/Core.hh>
#include <starlight/event/EventHandlerSentinel.hh>
#include <starlight/renderer/Renderer.hh>
#include <starlight/renderer/camera/Camera.hh>
#include <starlight/app/scene/SceneParser.hh>
#include <starlight/app/Engine.hh>

#include "Cameras.hh"
#include "ui/UserInterface.hh"
#include "Config.hh"

namespace sle {

class Application : public sl::Engine {
public:
    explicit Application(const Config& config);

private:
    void update(float frameTime) override;

    void initEvents();

    Config m_config;
    sl::EventHandlerSentinel m_eventSentinel;
    Cameras m_cameras;

    UserInterface m_userInterface;
    sl::SceneParser m_sceneParser;
};

}  // namespace sle
