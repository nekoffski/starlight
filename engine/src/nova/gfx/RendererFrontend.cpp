#include "RendererFrontend.h"

#include "nova/math/Glm.h"

#include "nova/core/Memory.hpp"
#include "nova/event/WindowResized.h"
#include "nova/event/Input.h"

#include "EulerCamera.h"

#include "nova/gfx/TextureManager.h"

namespace nova::gfx {

RendererFrontend::RendererFrontend(RendererBackend* backend) : m_backend(backend) {
    event::EventManager::get().registerObserver(this);

    m_texture1 = gfx::TextureManager::get().load("texture1", "cobblestone.png");
    m_texture2 = gfx::TextureManager::get().load("texture2", "paving.png");

    m_activeTexture = m_texture1;
}

RendererFrontend::~RendererFrontend() { event::EventManager::get().unregisterObserver(this); }

bool RendererFrontend::drawFrame(
    const RenderPacket& renderPacket, const Camera& camera, float deltaTime
) {
    if (m_backend->beginFrame(deltaTime)) {
        gfx::GlobalState globalState;

        auto modelMatrix = glm::mat4{1.0f};

        modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3{0.0f, 0.0f, 1.0f});

        globalState.projectionMatrix = camera.getProjectionMatrix();
        globalState.viewMatrix       = camera.getViewMatrix();
        globalState.viewPosition     = camera.getPosition();
        globalState.ambientColor     = glm::vec4{1.0f};
        globalState.mode             = 0;

        GeometryRenderData renderData;
        renderData.model       = modelMatrix;
        renderData.deltaTime   = deltaTime;
        renderData.objectId    = 0;
        renderData.textures[0] = m_activeTexture;

        // TODO: consider pasing delta timer directly
        m_backend->updateGlobalState(globalState);
        m_backend->updateObject(renderData);

        return m_backend->endFrame(deltaTime);
    }

    return true;
}

void RendererFrontend::onEvent(event::EventWrapper& eventWrapper) {
    if (eventWrapper.is<event::WindowResized>()) {
        // TODO: handle changing viewport as different event, viewport.size != window.size (GUI)
        LOG_TRACE("Window resized, handling viewport change on renderer backend");

        const auto event = eventWrapper.get<event::WindowResized>();
        m_backend->onViewportResize(event.width, event.height);
    } else if (eventWrapper.is<event::KeyEvent>()) {
        const auto event = eventWrapper.get<event::KeyEvent>();

        if (event.action == core::KeyAction::press) {
            if (event.key == NOVA_KEY_4) {
                LOG_TRACE("Changing current texture to 1");
                m_activeTexture = m_texture1;
                m_activeTexture->generation++;
            } else if (event.key == NOVA_KEY_5) {
                LOG_TRACE("Changing current texture to 2");
                m_activeTexture = m_texture2;
                m_activeTexture->generation++;
            }
        }
    }
}

}  // namespace nova::gfx
