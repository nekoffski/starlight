#include <starlight/framework/graphics/LowLevelRenderer.h>

namespace starl::framework::graphics {

void LowLevelRenderer::init() {
    m_graphicsContext->init();
}

void LowLevelRenderer::renderVertexArray(std::shared_ptr<gpu::VertexArray>& vertexArray) {
    vertexArray->bind();
    m_renderAPI->drawArrays(0, 0, 3);
    vertexArray->unbind();
}

void LowLevelRenderer::begin() {
    m_graphicsContext->clearBuffers();
    m_renderAPI->clearColor(1.0f, 1.0f, 0.4f, 1.0f);
}

void LowLevelRenderer::end() {
    m_graphicsContext->swapBuffers();
}
}
