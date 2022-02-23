#include "Renderer.h"

#include <kc/core/Log.h>

#include "sl/event/EventManager.h"

namespace sl::gfx {

Renderer::Renderer(gfx::RenderApi* renderApi, const ViewFrustum::Viewport& viewport)
    : EventListener("Renderer"), m_renderApi(renderApi), m_viewport(viewport) {
    sl::event::EventManager::get().registerListener(this);

    m_renderApi->enable(STARL_DEPTH_TEST);
    // m_renderApi->enable(STARL_BLEND);
    m_renderApi->setCullFace(STARL_BACK);
    m_renderApi->setDepthFunc(STARL_LEQUAL);
    m_renderApi->setBlendFunc(STARL_SRC_ALPHA, STARL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::renderVertexArray(gfx::VertexArray& vertexArray) {
    // to optimize! -> BRANCHING!!!
    if (auto indices = vertexArray.getIndicesCount(); indices != 0u)
        m_renderApi->drawElements(STARL_TRIANGLES, indices, STARL_UNSIGNED_INT);
    else
        m_renderApi->drawArrays(STARL_TRIANGLES, 0, vertexArray.getVerticesCount());
}
}  // namespace sl::gfx
