#pragma once

#include <kc/event/EventListener.h>

#include "sl/gfx/Renderer.h"

namespace sl::app {

struct Application : kc::event::EventListener {
    explicit Application() : EventListener("Application") {}

    virtual void update(float deltaTime, float now) = 0;
    virtual void render(gfx::Renderer& renderer) = 0;

    virtual bool isRunning() const = 0;

    virtual void forceStop() = 0;
};

}  // namespace sl::app
