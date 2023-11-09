#include "RendererFrontend.h"

#include <imgui.h>

#include "starlight/core/math/Glm.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/WindowManager.h"

#include <glm/gtc/type_ptr.hpp>

#include "camera/EulerCamera.h"

#include "starlight/resource/ResourceManager.h"

namespace sl {

RendererFrontend::RendererFrontend(RendererBackend* backend) :
    m_backend(backend), m_materialShader(nullptr), m_uiShader(nullptr),
    m_renderMode(RenderMode::standard), m_frameNumber(0ul) {}

RendererFrontend::~RendererFrontend() {}

bool RendererFrontend::renderFrame(float deltaTime) {
    m_frameNumber++;

    if (m_backend->beginFrame(deltaTime)) {
        for (auto& [renderPassId, renderPassCallback] : m_renderPasses)
            m_backend->renderPass(renderPassId, renderPassCallback);
        m_renderPasses.clear();
        m_backend->endFrame(deltaTime);
    }

    return true;
}

void RendererFrontend::addUIPass(std::function<void()>&& callback) {
    m_renderPasses.push_back(RenderPass(builtinRenderPassUI, [&]() {
        m_backend->renderUI(std::move(callback));
    }));
}

void RendererFrontend::addMainPass(
  RenderPacket& renderPacket, const Camera& camera
) {
    m_renderPasses.push_back(RenderPass(builtinRenderPassWorld, [&]() {
        glm::vec4 ambientColor(0.3f, 0.3f, 0.3f, 1.0f);

        m_materialShader->use();

        auto viewMatrix       = camera.getViewMatrix();
        auto projectionMatrix = camera.getProjectionMatrix();
        auto viewPosition     = camera.getPosition();

        m_materialShader->setGlobalUniforms([&](auto self) {
            self->setUniform("view", viewMatrix);
            self->setUniform("projection", projectionMatrix);
            self->setUniform("viewPosition", viewPosition);
            self->setUniform("ambientColor", ambientColor);
            self->setUniform("renderMode", static_cast<int>(m_renderMode));
        });

        for (auto& geometryRenderData : renderPacket.geometries) {
            auto& material = geometryRenderData.geometry->getProperties().material;

            material->applyUniforms(m_frameNumber);

            m_materialShader->setLocalUniforms([&](auto self) {
                self->setUniform("model", geometryRenderData.model);
            });

            m_backend->drawGeometry(geometryRenderData);
        }
    }));
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
