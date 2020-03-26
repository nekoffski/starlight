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
    :   Renderer(lowLevelRenderer)
{
    auto vertexBuffer = platform::gpu::VertexBuffer::create(cubemapVertices, sizeof(cubemapVertices), 36);
    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = platform::gpu::VertexArray::create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void CubemapRenderer::render() {
    m_cubemapShader->enable();
    m_cubemapShader->setUniform("projection", m_lowLevelRenderer.getProjectionMatrix());
    m_cubemapShader->setUniform("view", m_camera->getViewMatrix());
    m_cubemap->bind();
    m_lowLevelRenderer.renderVertexArrayWithDepthMaskDisabled(m_cubemapVertexArray);
    m_cubemap->unbind();
    m_cubemapShader->disable();
}
}
