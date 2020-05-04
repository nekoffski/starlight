#include <starlight/framework/graphics/LowLevelRenderer.h>

#include <starlight/core/log/Logger.h>

namespace starl::framework::graphics {

void LowLevelRenderer::init() {
    LOG(INFO) << "initializing";
    m_graphicsContext->init();

    m_renderAPI->enable(STARL_DEPTH_TEST);
    m_renderAPI->setDepthFunc(STARL_LEQUAL);
    m_renderAPI->enable(STARL_BLEND);
    m_renderAPI->setBlendFunc(STARL_SRC_ALPHA, STARL_ONE_MINUS_SRC_ALPHA);
}

void LowLevelRenderer::renderVertexArray(std::shared_ptr<platform::gpu::VertexArray>& vertexArray) {
    vertexArray->bind();

    // to optimize!
    auto indices = vertexArray->getIndicesCount();
    if (indices != 0u) {
        m_renderAPI->drawElements(STARL_TRIANGLES, indices, STARL_UNSIGNED_INT);
    } else {
        m_renderAPI->drawArrays(STARL_TRIANGLES, 0, vertexArray->getVerticesCount());
    }
    vertexArray->unbind();
}

void LowLevelRenderer::renderVertexArrayWithDepthMaskDisabled(std::shared_ptr<platform::gpu::VertexArray>& vertexArray) {
    m_renderAPI->disableDepthMask();
    renderVertexArray(vertexArray);
    m_renderAPI->enableDepthMask();
}

void LowLevelRenderer::begin() {
    m_graphicsContext->clearBuffers(STARL_DEPTH_BUFFER_BIT | STARL_COLOR_BUFFER_BIT);
    // TODO: extract as another method
    //m_renderAPI->clearColor(1.0f, 1.0f, 0.4f, 1.0f);
}

void LowLevelRenderer::end() {
    m_graphicsContext->swapBuffers();
}
}
