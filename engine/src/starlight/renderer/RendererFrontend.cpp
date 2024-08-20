#include "RendererFrontend.h"

#include "starlight/core/math/Core.h"
#include "starlight/core/memory/Memory.hpp"

#include "RenderPacket.h"
#include "camera/EulerCamera.h"

namespace sl {

RendererFrontend::RendererFrontend(Context& context) :
    m_backend(context.getWindow(), context.getConfig()),
    m_renderMode(RenderMode::standard), m_framesSinceResize(0u), m_resizing(false),
    m_viewportSize(context.getWindow().getFramebufferSize()),
    m_shaderManager(m_backend), m_textureManager(m_backend),
    m_skyboxManager(m_backend) {}

void RendererFrontend::init(std::span<RenderView*> renderViews) {
    m_renderViews.assign(renderViews.begin(), renderViews.end());

    auto backendProxy    = m_backend.getProxy();
    auto resourcePools   = m_backend.getResourcePools();
    const auto viewCount = m_renderViews.size();

    RenderView::InitProperties initProperties{ .viewportSize = m_viewportSize };

    for (u32 i = 0; i < viewCount; ++i) {
        initProperties.hasPreviousView = (i != 0);
        initProperties.hasNextView     = (i != viewCount - 1);
        m_renderViews[i]->init(*backendProxy, *resourcePools, initProperties);
    }
}

FrameStatistics RendererFrontend::getFrameStatistics() { return m_frameStatistics; }

RendererBackend& RendererFrontend::getRendererBackend() { return m_backend; }

void RendererFrontend::renderFrame(float deltaTime, const RenderPacket& packet) {
    m_frameStatistics.frameNumber++;
    m_frameStatistics.deltaTime = deltaTime;

    if (m_resizing) [[unlikely]] {
        static constexpr u16 requiredFramesSinceResize = 30u;

        LOG_TRACE(
          "Resizing viewport, frame dropped {}/{}", m_framesSinceResize,
          requiredFramesSinceResize
        );

        if (m_framesSinceResize++ >= requiredFramesSinceResize) {
            m_resizing          = false;
            m_framesSinceResize = 0;
        } else {
            m_frameStatistics.renderedVertices = 0;
        }
    }

    m_frameStatistics.renderedVertices = m_backend.renderFrame(deltaTime, [&]() {
        RenderProperties renderProperties{
            m_renderMode, m_frameStatistics.frameNumber
        };
        auto backendProxy = m_backend.getProxy();

        for (auto& view : m_renderViews)
            view->render(*backendProxy, packet, renderProperties, deltaTime);
    });
}

void RendererFrontend::setRenderMode(RenderMode mode) {
    LOG_TRACE("Render mode set to: {}", mode);  // TODO: toString{}
    m_renderMode = mode;
}

ResourcePools* RendererFrontend::getResourcePools() {
    return m_backend.getResourcePools();
}

void RendererFrontend::onViewportResize(Vec2<u32> viewportSize) {
    m_resizing = true;
    m_backend.onViewportResize(viewportSize);

    auto backendProxy = m_backend.getProxy();
    for (auto& view : m_renderViews)
        view->onViewportResize(*backendProxy, viewportSize);
}

}  // namespace sl
