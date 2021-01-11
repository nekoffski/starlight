#include "SkyboxSystem.h"

#include "sl/graphics/Cubemap.h"
#include "sl/graphics/Renderer.h"
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

namespace sl::scene::systems {

SkyboxSystem::SkyboxSystem(std::shared_ptr<graphics::Renderer> renderer)
    : m_renderer(renderer) {

    auto vertexBuffer = graphics::buffer::VertexBuffer::factory->create(cubemapVertices, sizeof(cubemapVertices), 36);
    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = graphics::buffer::VertexArray::factory->create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void SkyboxSystem::render(std::shared_ptr<sl::graphics::Cubemap> cubemap, std::shared_ptr<graphics::Shader> cubemapShader,
    std::shared_ptr<graphics::camera::Camera> camera) {

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
