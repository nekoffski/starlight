#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "nova/core/Memory.hpp"
#include "nova/event/Event.h"
#include "nova/event/WindowResized.h"

#include "RendererBacked.h"
#include "EulerCamera.h"

// TODO: implement texture loader

namespace nova::gfx {

struct RenderPacket {};

class RendererFrontend : public event::EventObserver {
   public:
    explicit RendererFrontend(RendererBackend* backend) : m_backend(backend) {
        event::EventManager::get().registerObserver(this);
    }

    virtual ~RendererFrontend() { event::EventManager::get().unregisterObserver(this); }

    bool drawFrame(const RenderPacket& renderPacket, const Camera& camera, float deltaTime) {
        if (m_backend->beginFrame(deltaTime)) {
            gfx::GlobalState globalState;

            // rotate

            static float angle         = 0.0f;
            constexpr float deltaAngle = 1.0f;

            // angle += deltaAngle;

            if (angle >= 360.0f) angle = 0.0f;

            auto modelMatrix = glm::mat4{1.0f};

            modelMatrix =
                glm::rotate(modelMatrix, glm::radians(angle), glm::vec3{0.0f, 0.0f, 1.0f});

            // why?
            // view = glm::inverse(view);

            globalState.projectionMatrix = camera.getProjectionMatrix();
            globalState.viewMatrix       = camera.getViewMatrix();
            globalState.viewPosition     = camera.getPosition();
            globalState.ambientColor     = glm::vec4{1.0f};
            globalState.mode             = 0;

            GeometryRenderData renderData;
            renderData.model     = modelMatrix;
            renderData.deltaTime = deltaTime;
            renderData.objectId  = 0;

            // TODO: consider pasing delta timer directly
            m_backend->updateGlobalState(globalState);
            m_backend->updateObject(renderData);

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
