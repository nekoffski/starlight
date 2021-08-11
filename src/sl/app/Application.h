#pragma once

#include <xvent/EventListener.h>

#include "sl/gfx/Renderer.h"

namespace sl::app {

struct Application : xvent::EventListener {
    explicit Application()
        : xvent::EventListener("Application") { }

    virtual void update(float deltaTime, float now) = 0;
    virtual void render(gfx::Renderer& renderer) = 0;

    virtual bool isRunning() const = 0;

    virtual void onStart() = 0;
    virtual void onStop() = 0;
    virtual void forceStop() = 0;
};

}
