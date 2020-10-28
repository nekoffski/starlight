#include "Renderer.h"

#include "sl/core/log/Logger.h"

namespace sl::rendering {

void Renderer::init() {
    SL_INFO("initializing");
    m_graphicsContext->init();
    m_graphicsContext->setViewport(m_viewport.width, m_viewport.height);

    m_renderAPI->enable(STARL_DEPTH_TEST);
    m_renderAPI->setDepthFunc(STARL_LEQUAL);
    m_renderAPI->enable(STARL_BLEND);
    m_renderAPI->setBlendFunc(STARL_SRC_ALPHA, STARL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::renderVertexArray(std::shared_ptr<platform::gpu::VertexArray> vertexArray) {
    // TODO: this function should let call a lot of draw calls with same vao binded
    // vertexArray->bind();

    // to optimize! -> BRANCHING!!!
    auto indices = vertexArray->getIndicesCount();
    if (indices != 0u)
        m_renderAPI->drawElements(STARL_TRIANGLES, indices, STARL_UNSIGNED_INT);
    else
        m_renderAPI->drawArrays(STARL_TRIANGLES, 0, vertexArray->getVerticesCount());
    // vertexArray->unbind();
}

void Renderer::renderVertexArrayWithDepthMaskDisabled(std::shared_ptr<platform::gpu::VertexArray> vertexArray) {
    m_renderAPI->disableDepthMask();
    renderVertexArray(vertexArray);
    m_renderAPI->enableDepthMask();
}

void Renderer::begin() {
    m_graphicsContext->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);
    // TODO: extract as another method
    //m_renderAPI->clearColor(1.0f, 1.0f, 0.4f, 1.0f);
}

void Renderer::end() {
    m_graphicsContext->swapBuffers();
}
}
