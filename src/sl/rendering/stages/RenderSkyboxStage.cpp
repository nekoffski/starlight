#include "RenderSkyboxStage.h"

#include <kc/core/Profiler.h>

#include "sl/geom/CubemapVertices.h"
#include "sl/gfx/BufferManager.h"

namespace sl::rendering::stages {

RenderSkyboxStage::RenderSkyboxStage() {
    auto vertexBuffer = gfx::BufferManager::get().createVertexBuffer(
        geom::cubemapVertices, sizeof(geom::cubemapVertices), 36
    );

    vertexBuffer->addMemoryOffsetScheme(3, STARL_FLOAT, sizeof(float));

    m_cubemapVertexArray = gfx::BufferManager::get().createVertexArray();
    m_cubemapVertexArray->addVertexBuffer(vertexBuffer);
}

void RenderSkyboxStage::execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) {
    PROFILE_FUNCTION();

    if (not scene.skybox.has_value()) return;

    auto& skybox = scene.skybox;

    skybox->cubemap->bind(0);

    skybox->shader->enable();
    skybox->shader->setUniform("projectionMatrix", scene.camera->getProjectionMatrix());
    skybox->shader->setUniform("viewMatrix", scene.camera->getViewMatrix());

    m_cubemapVertexArray->bind();

    auto settings            = renderer.getSettings();
    settings.enableDepthMask = false;

    renderer.setTemporarySettings(settings);
    renderer.renderVertexArray(*m_cubemapVertexArray);
    renderer.restoreSettings();

    skybox->cubemap->unbind();

    m_cubemapVertexArray->unbind();
    skybox->shader->disable();
}

}  // namespace sl::rendering::stages