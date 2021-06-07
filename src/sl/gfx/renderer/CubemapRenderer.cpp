#include "CubemapRenderer.h"

#include "sl/gfx/Cubemap.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/buffer/VertexArray.h"
#include "sl/gfx/buffer/VertexBuffer.h"

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

namespace sl::gfx::renderer {

CubemapRenderer::CubemapRenderer(LowLevelRenderer& renderer)
    : m_renderer(renderer) {

    auto vertexBuffer = buffer::VertexBuffer::factory->create(cubemapVertices, sizeof(cubemapVertices), 36);
    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = buffer::VertexArray::factory->create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void CubemapRenderer::render(Cubemap& cubemap, Shader& cubemapShader, camera::Camera& camera) {

    cubemapShader.enable();
    cubemapShader.setUniform("projection", camera.getProjectionMatrix());
    cubemapShader.setUniform("view", camera.getViewMatrix());

    m_cubemapVertexArray->bind();

    auto settings = m_renderer.getSettings();
    settings.enableDepthMask = false;

    m_renderer.setTemporarySettings(settings);
    m_renderer.renderVertexArray(m_cubemapVertexArray);
    m_renderer.restoreSettings();

    m_cubemapVertexArray->unbind();
    cubemapShader.disable();
}
}
