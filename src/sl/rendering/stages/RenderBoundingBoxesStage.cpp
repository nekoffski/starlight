#include "RenderBoundingBoxesStage.h"

#include <kc/core/Profiler.h>

#include "sl/core/Colors.h"
#include "sl/rendering/utils/Misc.h"
#include "sl/scene/components/ModelComponent.h"

namespace sl::rendering::stages {

using namespace sl::scene::components;

RenderBoundingBoxesStage::RenderBoundingBoxesStage()
    : m_boundingBoxShader(sl::glob::Globals::get().shaders->singleColorShader) {}

void RenderBoundingBoxesStage::
    execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) {
    PROFILE_FUNCTION();

    setShaderTransforms(*scene.camera);
    prepareRenderer(renderer);

    auto [models, transforms] =
        scene.ecsRegistry.getComponentsViews<ModelComponent, TransformComponent>();

    m_boundingBoxShader->enable();

    for (auto& model : models) {
        if (auto& boundingBox = model.boundingBox; boundingBox && model.renderBoundingBox) {
            const auto& ownerEntityId = model.ownerEntityId;
            const auto& modelMatrix   = utils::getModelMatrix(ownerEntityId, transforms);

            m_boundingBoxShader->setUniform("modelMatrix", modelMatrix);
            m_boundingBoxShader->setUniform("color", core::color::red);

            renderBoundingBox(renderer, *boundingBox);
        }
    }

    m_boundingBoxShader->disable();

    renderer.restoreSettings();
}

void RenderBoundingBoxesStage::setShaderTransforms(cam::Camera& camera) {
    m_boundingBoxShader->setUniform("projectionMatrix", camera.getProjectionMatrix());
    m_boundingBoxShader->setUniform("viewMatrix", camera.getViewMatrix());
}

void RenderBoundingBoxesStage::prepareRenderer(gfx::Renderer& renderer) {
    auto settings        = renderer.getSettings();
    settings.polygonMode = STARL_LINE;
    renderer.setTemporarySettings(settings);
}

void RenderBoundingBoxesStage::renderBoundingBox(
    gfx::Renderer& renderer, physx::BoundingBox& boundingBox
) {
    auto vao = boundingBox.getVertexArray();

    vao->bind();
    renderer.renderVertexArray(*vao);
    vao->unbind();
}

}  // namespace sl::rendering::stages