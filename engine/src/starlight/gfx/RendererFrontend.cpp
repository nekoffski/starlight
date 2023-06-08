#include "RendererFrontend.h"

#include "starlight/math/Glm.h"

#include "starlight/core/Memory.hpp"
#include "starlight/event/WindowResized.h"
#include "starlight/event/Input.h"

#include "EulerCamera.h"

#include "starlight/gfx/MaterialManager.h"
#include "starlight/gfx/TextureManager.h"

namespace sl::gfx {

RendererFrontend::RendererFrontend(RendererBackend* backend) : m_backend(backend) {
    event::EventManager::get().registerObserver(this);

    m_texture1 = gfx::TextureManager::get().load("cobblestone.png");
    m_texture2 = gfx::TextureManager::get().load("paving.png");

    m_activeTexture = m_texture1;

    // m_material = gfx::MaterialManager::get().load("test");
    m_material = gfx::MaterialManager::get().getDefaultMaterial();
}

RendererFrontend::~RendererFrontend() { event::EventManager::get().unregisterObserver(this); }

bool RendererFrontend::drawFrame(
    const RenderPacket& renderPacket, const Camera& camera, float deltaTime
) {
    if (m_backend->beginFrame(deltaTime)) {
        gfx::GlobalState globalState;

        globalState.projectionMatrix = camera.getProjectionMatrix();
        globalState.viewMatrix       = camera.getViewMatrix();
        globalState.viewPosition     = camera.getPosition();
        globalState.ambientColor     = glm::vec4{1.0f};
        globalState.mode             = 0;

        m_backend->updateGlobalState(globalState);

        for (auto& geometry : renderPacket.geometries) m_backend->drawGeometry(geometry);

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
            if (event.key == SL_KEY_4) {
                LOG_TRACE("Changing current texture to 1");
                m_material->diffuseMap.texture = m_texture1;
            } else if (event.key == SL_KEY_5) {
                LOG_TRACE("Changing current texture to 2");
                m_material->diffuseMap.texture = m_texture2;
            }
        }
    }
}

}  // namespace sl::gfx
