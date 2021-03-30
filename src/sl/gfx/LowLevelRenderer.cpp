#include "LowLevelRenderer.h"

#include "sl/core/Logger.h"

namespace sl::gfx {

LowLevelRenderer::LowLevelRenderer(std::shared_ptr<gfx::GraphicsContext> gfxContext,
    std::unique_ptr<gfx::RenderApi> renderApi, ViewFrustum::Viewport viewport)
    : m_gfxContext(gfxContext)
    , m_renderApi(std::move(renderApi))
    , m_viewport(std::move(viewport)) {

    SL_INFO("initializing");
    m_gfxContext->setViewport(m_viewport);

    m_renderApi->enable(STARL_DEPTH_TEST);
    m_renderApi->setDepthFunc(STARL_LEQUAL);
    m_renderApi->enable(STARL_BLEND);
    m_renderApi->setBlendFunc(STARL_SRC_ALPHA, STARL_ONE_MINUS_SRC_ALPHA);
}

void LowLevelRenderer::renderVertexArray(std::shared_ptr<gfx::buffer::VertexArray> vertexArray) {
    // to optimize! -> BRANCHING!!!
    if (auto indices = vertexArray->getIndicesCount(); indices != 0u)
        m_renderApi->drawElements(STARL_TRIANGLES, indices, STARL_UNSIGNED_INT);
    else
        m_renderApi->drawArrays(STARL_TRIANGLES, 0, vertexArray->getVerticesCount());
}
}
