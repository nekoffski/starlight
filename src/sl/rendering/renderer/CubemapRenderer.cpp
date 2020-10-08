#include "CubemapRenderer.h"

#include "sl/platform/gpu/VertexArray.h"
#include "sl/platform/gpu/VertexBuffer.h"
#include "sl/platform/shader/Shader.h"
#include "sl/platform/texture/Cubemap.h"

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

namespace sl::rendering::renderer {

CubemapRenderer::CubemapRenderer(lowlevel::LowLevelRenderer& lowLevelRenderer)
    : Renderer(lowLevelRenderer) {
    auto vertexBuffer = platform::gpu::VertexBuffer::create(cubemapVertices, sizeof(cubemapVertices), 36);
    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = platform::gpu::VertexArray::create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void CubemapRenderer::render(const std::shared_ptr<platform::texture::Cubemap>& cubemap,
    const std::shared_ptr<platform::shader::Shader>& cubemapShader,
    const std::shared_ptr<rendering::camera::Camera>& camera) {

    cubemapShader->enable();
    cubemapShader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());
    cubemapShader->setUniform("view", camera->getViewMatrix());

    // cubemap->bind();
    m_cubemapVertexArray->bind();

    m_lowLevelRenderer.renderVertexArrayWithDepthMaskDisabled(m_cubemapVertexArray);

    m_cubemapVertexArray->unbind();
    // cubemap->unbind();
    cubemapShader->disable();
}
}
