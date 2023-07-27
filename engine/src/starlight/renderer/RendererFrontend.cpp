#include "RendererFrontend.h"

#include "starlight/core/math/Glm.h"
#include "starlight/core/Memory.hpp"
#include "starlight/core/window/WindowManager.h"

#include "camera/EulerCamera.h"

namespace sl {

RendererFrontend::RendererFrontend(RendererBackend* backend) : m_backend(backend) {}

RendererFrontend::~RendererFrontend() {}

bool RendererFrontend::drawFrame(
    const RenderPacket& renderPacket, const Camera& camera, float deltaTime
) {
    if (m_backend->beginFrame(deltaTime)) {
        m_backend->renderPass(builtinRenderPassWorld, [&] {
            GlobalState globalState;
            globalState.projectionMatrix = camera.getProjectionMatrix();
            globalState.viewMatrix       = camera.getViewMatrix();
            globalState.viewPosition     = camera.getPosition();
            globalState.ambientColor     = glm::vec4{1.0f};
            globalState.mode             = 0;

            m_backend->updateGlobalWorldState(globalState);

            for (auto& geometry : renderPacket.geometries) m_backend->drawGeometry(geometry);
        });

        m_backend->renderPass(builtinRenderPassUI, [&] {
            const auto& [w, h] = WindowManager::get().getSize();

            Mat4f projection = math::ortho(
                0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -100.0f, 100.0f
            );
            Mat4f view = math::inverse(identityMatrix);

            m_backend->updateGlobalUIState(projection, view, 0);

            for (const auto& uiGeometry : renderPacket.uiGeometries)
                m_backend->drawGeometry(uiGeometry);
        });

        m_backend->endFrame(deltaTime);
    }

    return true;
}

}  // namespace sl
