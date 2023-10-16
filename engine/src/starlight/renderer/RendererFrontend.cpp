#include "RendererFrontend.h"

#include "starlight/core/math/Glm.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/WindowManager.h"

#include <glm/gtc/type_ptr.hpp>

#include "camera/EulerCamera.h"

#include "starlight/resource/ResourceManager.h"

namespace sl {

RendererFrontend::RendererFrontend(RendererBackend* backend) :
    m_backend(backend), m_materialShader(nullptr), m_uiShader(nullptr) {}

RendererFrontend::~RendererFrontend() {}

bool RendererFrontend::drawFrame(
  RenderPacket& renderPacket, const Camera& camera, float deltaTime
) {
    if (m_backend->beginFrame(deltaTime)) {
        const auto mainPass = [&] {
            GlobalState globalState;
            globalState.projectionMatrix = camera.getProjectionMatrix();
            globalState.viewMatrix       = camera.getViewMatrix();
            globalState.viewPosition     = camera.getPosition();
            globalState.ambientColor     = glm::vec4{ 1.0f };
            globalState.mode             = 0;

            m_materialShader->use();
            m_materialShader->bindGlobals();
            LOG_WARN("Set uniform start");
            m_materialShader->setUniform(
              "view", glm::value_ptr(globalState.viewMatrix)
            );
            m_materialShader->setUniform(
              "projection", glm::value_ptr(globalState.projectionMatrix)
            );

            LOG_WARN("Set uniform stop");
            m_materialShader->applyGlobals();

            for (auto& geometry : renderPacket.geometries) {
                m_materialShader->bindInstance(
                  geometry.geometry->material->internalId
                );
                LOG_WARN("Local start");
                m_materialShader->setUniform(
                  "diffuse_colour",
                  glm::value_ptr(geometry.geometry->material->diffuseColor)
                );

                auto t = ResourceManager::get().getDefaultTexture();

                m_materialShader->setUniform("diffuse_texture", t);
                m_materialShader->applyInstance();
                m_materialShader->setUniform(
                  "model", glm::value_ptr(geometry.model)
                );
                LOG_WARN("Local stop");

                m_backend->drawGeometry(geometry);
            }
        };
        LOG_ERROR("World pass start");
        m_backend->renderPass(builtinRenderPassWorld, mainPass);
        LOG_ERROR("World pass end");

        const auto uiPass = [&] {
            const auto& [w, h] = WindowManager::get().getSize();

            Mat4f projection = math::ortho(
              0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, -100.0f,
              100.0f
            );
            Mat4f view = identityMatrix;

            m_uiShader->use();
            m_uiShader->bindGlobals();
            m_uiShader->setUniform("projection", glm::value_ptr(projection));
            m_uiShader->setUniform("view", glm::value_ptr(view));
            m_uiShader->applyGlobals();

            for (auto& uiGeometry : renderPacket.uiGeometries) {
                m_uiShader->bindInstance(uiGeometry.geometry->material->internalId);
                m_uiShader->setUniform(
                  "diffuse_colour",
                  glm::value_ptr(uiGeometry.geometry->material->diffuseColor)
                );

                m_uiShader->setUniform(
                  "diffuse_texture",
                  uiGeometry.geometry->material->diffuseMap.texture
                );
                m_uiShader->applyInstance();
                m_uiShader->setUniform("model", glm::value_ptr(uiGeometry.model));

                m_backend->drawGeometry(uiGeometry);
            }
        };
        m_backend->renderPass(builtinRenderPassUI, uiPass);

        m_backend->endFrame(deltaTime);
    }

    return true;
}

void RendererFrontend::setCoreShaders(Shader* uiShader, Shader* materialShader) {
    m_uiShader       = uiShader;
    m_materialShader = materialShader;
}

}  // namespace sl
