#include "RendererFrontend.h"

#include <imgui.h>

#include "starlight/core/math/Glm.h"
#include "starlight/core/memory/Memory.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "camera/EulerCamera.h"

namespace sl {

RendererFrontend::RendererFrontend(
  Window& window, const Config& config, Camera* camera
) :
    m_backend(window, config),
    m_camera(camera), m_materialShader(nullptr), m_uiShader(nullptr),
    m_renderMode(RenderMode::standard), m_frameNumber(0ul), m_framesSinceResize(0u),
    m_resizing(false) {
    const auto [w, h] = window.getSize();
    m_viewportWidth   = w;
    m_viewportHeight  = h;
}

RendererFrontend::~RendererFrontend() {}

FrameStatistics RendererFrontend::renderFrame(float deltaTime) {
    m_frameNumber++;
    u64 totalVerticesRendered = 0u;

    if (m_resizing) {
        static constexpr u16 requiredFramesSinceResize = 30u;

        LOG_TRACE("Resizing viewport, frame dropped");

        if (m_framesSinceResize++ >= requiredFramesSinceResize) {
            m_resizing          = false;
            m_framesSinceResize = 0;
        } else {
            return FrameStatistics{
                .renderedVertices = 0, .frameNumber = m_frameNumber
            };
        }
    }

    if (m_backend.beginFrame(deltaTime)) {
        for (auto& [renderPassId, renderPassCallback] : m_renderPasses) {
            auto renderPassStats =
              m_backend.renderPass(renderPassId, renderPassCallback);
            totalVerticesRendered += renderPassStats.renderedVertices;
        }
        m_renderPasses.clear();

        m_backend.endFrame(deltaTime);
    }
    return FrameStatistics{
        .renderedVertices = totalVerticesRendered, .frameNumber = m_frameNumber
    };
}

void RendererFrontend::addUIPass(std::function<void()>&& callback) {
    m_renderPasses.push_back(RenderPass(builtinRenderPassUI, [&]() {
        m_backend.renderUI(std::move(callback));
    }));
}

void RendererFrontend::addMainPass(RenderPacket& renderPacket) {
    m_renderPasses.push_back(RenderPass(builtinRenderPassWorld, [&]() {
        glm::vec4 ambientColor(0.3f, 0.3f, 0.3f, 1.0f);

        m_materialShader->use();

        m_materialShader->setGlobalUniforms([&](Shader::UniformProxy& proxy) {
            proxy.set("view", m_camera->getViewMatrix());
            proxy.set("projection", m_camera->getProjectionMatrix());
            proxy.set("viewPosition", m_camera->getPosition());
            proxy.set("ambientColor", ambientColor);
            proxy.set("renderMode", static_cast<int>(m_renderMode));
        });

        for (auto& geometryRenderData : renderPacket.geometries) {
            auto& material = geometryRenderData.geometry->getProperties().material;

            material->applyUniforms(m_frameNumber);

            m_materialShader->setLocalUniforms([&](Shader::UniformProxy& proxy) {
                proxy.set("model", geometryRenderData.model);
            });

            m_backend.drawGeometry(geometryRenderData);
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

void RendererFrontend::setCamera(Camera* camera) { m_camera = camera; }

ResourcePools* RendererFrontend::getResourcePools() {
    return m_backend.getResourcePools();
}

void RendererFrontend::onViewportResize(u32 w, u32 h) {
    m_resizing = true;
    m_backend.onViewportResize(w, h);
}

}  // namespace sl
