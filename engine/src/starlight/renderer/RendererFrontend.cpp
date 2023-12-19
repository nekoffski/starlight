#include "RendererFrontend.h"

#include "starlight/core/math/Glm.h"
#include "starlight/core/memory/Memory.hpp"

#include "RenderPacket.h"
#include "camera/EulerCamera.h"

namespace sl {

RendererFrontend::RendererFrontend(
  Window& window, const Config& config, Camera* camera
) :
    m_backend(window, config),
    m_renderMode(RenderMode::standard), m_frameNumber(0ul), m_framesSinceResize(0u),
    m_resizing(false) {
    const auto [w, h] = window.getSize();
    m_viewportWidth   = w;
    m_viewportHeight  = h;
}

RendererFrontend::~RendererFrontend() {}

void RendererFrontend::init(std::span<RenderView*> renderViews) {
    m_renderViews.assign(renderViews.begin(), renderViews.end());

    auto backendProxy  = m_backend.getProxy();
    auto resourcePools = m_backend.getResourcePools();

    const auto viewCount = m_renderViews.size();

    RenderView::InitProperties initProperties{
        .viewportWidth = m_viewportWidth, .viewportHeight = m_viewportHeight
    };

    for (u32 i = 0; i < viewCount; ++i) {
        initProperties.hasPreviousView = (i != 0);
        initProperties.hasNextView     = (i != viewCount - 1);

        m_renderViews[i]->init(*backendProxy, *resourcePools, initProperties);
    }
}

FrameStatistics RendererFrontend::renderFrame(
  float deltaTime, std::span<Mesh> meshes
) {
    m_frameNumber++;

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

    const auto renderedVertices = m_backend.renderFrame(deltaTime, [&]() {
        RenderPacket renderPacket{ meshes, deltaTime, m_renderMode, m_frameNumber };

        auto backendProxy = m_backend.getProxy();
        for (auto& view : m_renderViews) view->render(*backendProxy, renderPacket);
    });
    return FrameStatistics{ renderedVertices, m_frameNumber };
}

void RendererFrontend::setRenderMode(RenderMode mode) {
    LOG_TRACE("Render mode set to: {}", mode);  // TODO: toString{}
    m_renderMode = mode;
}

ResourcePools* RendererFrontend::getResourcePools() {
    return m_backend.getResourcePools();
}

void RendererFrontend::onViewportResize(u32 w, u32 h) {
    m_resizing = true;

    auto backendProxy = m_backend.getProxy();

    for (auto& view : m_renderViews) view->onViewportResize(*backendProxy, w, h);
    m_backend.onViewportResize(w, h);
}

}  // namespace sl
