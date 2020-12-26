#include "Renderer.h"

#include "sl/core/Logger.h"

namespace sl::graphics {

void Renderer::init() {
    SL_INFO("initializing");
    m_graphicsContext->setViewport(m_viewport.width, m_viewport.height);
    calculateProjectionMatrix();

    m_renderApi->enable(STARL_DEPTH_TEST);
    m_renderApi->setDepthFunc(STARL_LEQUAL);
    m_renderApi->enable(STARL_BLEND);
    m_renderApi->setBlendFunc(STARL_SRC_ALPHA, STARL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::renderVertexArray(std::shared_ptr<graphics::buffer::VertexArray> vertexArray) {
    // to optimize! -> BRANCHING!!!
    if (auto indices = vertexArray->getIndicesCount(); indices != 0u)
        m_renderApi->drawElements(STARL_TRIANGLES, indices, STARL_UNSIGNED_INT);
    else
        m_renderApi->drawArrays(STARL_TRIANGLES, 0, vertexArray->getVerticesCount());
}

void Renderer::renderVertexArrayWithDepthMaskDisabled(std::shared_ptr<graphics::buffer::VertexArray> vertexArray) {
    m_renderApi->disableDepthMask();
    renderVertexArray(vertexArray);
    m_renderApi->enableDepthMask();
}
}
