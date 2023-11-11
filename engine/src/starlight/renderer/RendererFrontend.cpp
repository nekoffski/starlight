#include "RendererFrontend.h"

#include <imgui.h>

#include "starlight/core/math/Glm.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/WindowManager.h"

#include <glm/gtc/type_ptr.hpp>

#include "camera/EulerCamera.h"

namespace sl {

RendererFrontend::RendererFrontend(RendererBackend* backend) :
    m_backend(backend), m_materialShader(nullptr), m_uiShader(nullptr),
    m_renderMode(RenderMode::standard), m_frameNumber(0ul) {}

RendererFrontend::~RendererFrontend() {}

FrameStatistics RendererFrontend::renderFrame(float deltaTime) {
    m_frameNumber++;
    FrameStatistics stats;
    stats.frameNumber = m_frameNumber;

    if (m_backend->beginFrame(deltaTime)) {
        for (auto& [renderPassId, renderPassCallback] : m_renderPasses) {
            auto renderPassStats =
              m_backend->renderPass(renderPassId, renderPassCallback);
            stats.renderedVertices += renderPassStats.renderedVertices;
        }
        m_renderPasses.clear();
        m_backend->endFrame(deltaTime);
    }
    return stats;
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

        m_materialShader->setGlobalUniforms([&](Shader::UniformProxy& proxy) {
            proxy.set("view", camera.getViewMatrix());
            proxy.set("projection", camera.getProjectionMatrix());
            proxy.set("viewPosition", camera.getPosition());
            proxy.set("ambientColor", ambientColor);
            proxy.set("renderMode", static_cast<int>(m_renderMode));
        });

        for (auto& geometryRenderData : renderPacket.geometries) {
            auto& material = geometryRenderData.geometry->getProperties().material;

            material->applyUniforms(m_frameNumber);

            m_materialShader->setLocalUniforms([&](Shader::UniformProxy& proxy) {
                proxy.set("model", geometryRenderData.model);
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
