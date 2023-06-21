#include "RendererFrontend.h"

#include "starlight/core/math/Glm.h"
#include "starlight/core/Memory.hpp"

// #include "starlight/event/WindowResized.h"
// #include "starlight/event/Input.h"

#include "camera/EulerCamera.h"

// #include "starlight/renderer/MaterialManager.h"
// #include "starlight/renderer/TextureManager.h"

namespace sl {

RendererFrontend::RendererFrontend(RendererBackend* backend) : m_backend(backend) {}

RendererFrontend::~RendererFrontend() {}

bool RendererFrontend::drawFrame(
    const RenderPacket& renderPacket, const Camera& camera, float deltaTime
) {
    if (m_backend->beginFrame(deltaTime)) {
        GlobalState globalState;

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

}  // namespace sl
