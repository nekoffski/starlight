#include "RenderSkyboxStage.h"

#include "sl/core/Profiler.h"
#include "sl/geom/CubemapVertices.h"

namespace sl::rendering::stages {

RenderSkyboxStage::RenderSkyboxStage() {
    auto vertexBuffer = gfx::buffer::VertexBuffer::factory->create(
        geom::cubemapVertices, sizeof(geom::cubemapVertices), 36);

    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = gfx::buffer::VertexArray::factory->create();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void RenderSkyboxStage::execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) {
    SL_PROFILE_FUNCTION();

    if (not scene.skybox.has_value())
        return;

    auto& skybox = scene.skybox;

    skybox->shader->enable();
    skybox->shader->setUniform("projection", scene.camera->getProjectionMatrix());
    skybox->shader->setUniform("view", scene.camera->getViewMatrix());

    m_cubemapVertexArray->bind();

    auto settings = renderer.getSettings();
    settings.enableDepthMask = false;

    renderer.setTemporarySettings(settings);
    renderer.renderVertexArray(*m_cubemapVertexArray);
    renderer.restoreSettings();

    m_cubemapVertexArray->unbind();
    skybox->shader->disable();
}

}