#include "RendererFrontend.h"

#include "starlight/core/math/Glm.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/WindowManager.h"

#include <glm/gtc/type_ptr.hpp>

#include "camera/EulerCamera.h"

#include "starlight/resource/ResourceManager.h"

namespace sl {

RendererFrontend::RendererFrontend(RendererBackend* backend) :
    m_backend(backend), m_materialShader(nullptr), m_uiShader(nullptr),
    m_renderMode(RenderMode::standard) {}

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

            static float angle = 0.0f;
            angle += (0.3f * deltaTime);

            auto model =
              glm::rotate(identityMatrix, angle, Vec3f{ 0.0f, 1.0f, 0.0f });
            glm::vec4 ambientColor(0.3f, 0.3f, 0.3f, 1.0f);

            m_materialShader->use();

            m_materialShader->setGlobalUniforms([&](auto self) {
                self->setUniform("view", globalState.viewMatrix);
                self->setUniform("projection", globalState.projectionMatrix);
                self->setUniform("viewPosition", globalState.viewPosition);
                self->setUniform("ambientColor", ambientColor);
                self->setUniform("renderMode", static_cast<int>(m_renderMode));
            });

            for (auto& geometryRenderData : renderPacket.geometries) {
                auto& material = geometryRenderData.geometry->material;

                material->applyUniforms(m_materialShader);

                m_materialShader->setLocalUniforms([&](auto self) {
                    self->setUniform("model", model);
                });

                m_backend->drawGeometry(geometryRenderData);
            }
        };
        m_backend->renderPass(builtinRenderPassWorld, mainPass);

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

                // TODO: skip for now
                // m_backend->ddrawGeometry(uiGeometry);
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

void RendererFrontend::setRenderMode(RenderMode mode) {
    LOG_TRACE("Render mode set to: {}", mode);  // TODO: toString{}
    m_renderMode = mode;
}

}  // namespace sl
