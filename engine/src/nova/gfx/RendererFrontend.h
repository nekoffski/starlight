#pragma once

#include "nova/event/Event.h"
#include "nova/event/WindowResized.h"

#include "RendererBacked.h"

namespace nova::gfx {

struct RenderPacket {};

class RendererFrontend : public event::EventObserver {
   public:
    explicit RendererFrontend(RendererBackend* backend) : m_backend(backend) {
        event::EventManager::get().registerObserver(this);
    }

    virtual ~RendererFrontend() { event::EventManager::get().unregisterObserver(this); }

    bool drawFrame(const RenderPacket& renderPacket, float deltaTime) {
        if (m_backend->beginFrame(deltaTime)) {
            return m_backend->endFrame(deltaTime);
        }

        return true;
    }

    void onEvent(event::EventWrapper& eventWrapper) override {
        if (eventWrapper.is<event::WindowResized>()) {
            // TODO: handle changing viewport as different event, viewport.size != window.size (GUI)
            LOG_TRACE("Window resized, handling viewport change on renderer backend");

            const auto event = eventWrapper.get<event::WindowResized>();
            m_backend->onViewportResize(event.width, event.height);
        }
    }

   private:
    RendererBackend* m_backend;
};

}  // namespace nova::gfx
