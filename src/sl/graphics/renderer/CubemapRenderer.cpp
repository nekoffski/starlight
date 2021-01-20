#include "CubemapRenderer.h"

#include "sl/graphics/Cubemap.h"
#include "sl/graphics/LowLevelRenderer.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/buffer/VertexArray.h"
#include "sl/graphics/buffer/VertexBuffer.h"
#include "sl/platform/gpu/fwd.h"

// clang-format off
static float cubemapVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
};
// clang-format on

namespace sl::graphics::renderer {

CubemapRenderer::CubemapRenderer(std::shared_ptr<LowLevelRenderer> renderer)
    : m_renderer(renderer) {

    auto vertexBuffer = buffer::VertexBuffer::factory->create(cubemapVertices, sizeof(cubemapVertices), 36);
    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = buffer::VertexArray::factory->create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void CubemapRenderer::render(std::shared_ptr<Cubemap> cubemap, std::shared_ptr<Shader> cubemapShader,
    std::shared_ptr<camera::Camera> camera) {

    cubemapShader->enable();
    cubemapShader->setUniform("projection", m_renderer->getProjectionMatrix());
    cubemapShader->setUniform("view", camera->getViewMatrix());

    m_cubemapVertexArray->bind();

    auto settings = m_renderer->getSettings();
    settings.enableDepthMask = false;

    m_renderer->setTemporarySettings(settings);
    m_renderer->renderVertexArray(m_cubemapVertexArray);
    m_renderer->restoreSettings();

    m_cubemapVertexArray->unbind();
    cubemapShader->disable();
}
}
