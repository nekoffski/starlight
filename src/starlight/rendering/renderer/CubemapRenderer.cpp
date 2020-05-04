#include <starlight/rendering/renderer/CubemapRenderer.h>

#include <starlight/platform/gpu/VertexArray.h>
#include <starlight/platform/gpu/VertexBuffer.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/texture/Cubemap.h>

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

namespace starl::rendering::renderer {

CubemapRenderer::CubemapRenderer(framework::graphics::LowLevelRenderer& lowLevelRenderer)
    : Renderer(lowLevelRenderer) {
    auto vertexBuffer = platform::gpu::VertexBuffer::create(cubemapVertices, sizeof(cubemapVertices), 36);
    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = platform::gpu::VertexArray::create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void CubemapRenderer::render(const std::shared_ptr<platform::texture::Cubemap>& cubemap,
    const std::shared_ptr<platform::shader::Shader>& cubemapShader,
    const std::shared_ptr<framework::graphics::camera::Camera>& camera) {
    cubemapShader->enable();
    cubemapShader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());
    cubemapShader->setUniform("view", camera->getViewMatrix());
    cubemap->bind();
    m_lowLevelRenderer.renderVertexArrayWithDepthMaskDisabled(m_cubemapVertexArray);
    cubemap->unbind();
    cubemapShader->disable();
}
}
