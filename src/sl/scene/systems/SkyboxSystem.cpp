#include "SkyboxSystem.h"

#include "sl/platform/gpu/VertexArray.h"
#include "sl/platform/gpu/VertexBuffer.h"
#include "sl/platform/shader/Shader.h"
#include "sl/platform/texture/Cubemap.h"
#include "sl/graphics/Renderer.h"

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

namespace sl::scene::systems {

SkyboxSystem::SkyboxSystem(std::shared_ptr<graphics::Renderer> renderer)
    : m_renderer(renderer) {

    auto vertexBuffer = platform::gpu::VertexBuffer::create(cubemapVertices, sizeof(cubemapVertices), 36);
    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = platform::gpu::VertexArray::create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void SkyboxSystem::render(std::shared_ptr<platform::texture::Cubemap> cubemap, std::shared_ptr<platform::shader::Shader> cubemapShader,
    std::shared_ptr<graphics::camera::Camera> camera) {

    cubemapShader->enable();
    cubemapShader->setUniform("projection", m_renderer->getProjectionMatrix());
    cubemapShader->setUniform("view", camera->getViewMatrix());

    m_cubemapVertexArray->bind();
    m_renderer->renderVertexArrayWithDepthMaskDisabled(m_cubemapVertexArray);
    m_cubemapVertexArray->unbind();
    cubemapShader->disable();
}
}
