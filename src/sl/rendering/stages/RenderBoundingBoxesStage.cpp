#include "RenderBoundingBoxesStage.h"

#include <kc/core/Profiler.h>

#include "sl/core/Colors.h"
#include "sl/rendering/utils/Misc.h"

namespace sl::rendering::stages {

using namespace sl::scene::components;

RenderBoundingBoxesStage::RenderBoundingBoxesStage()
    : m_boundingBoxShader(sl::glob::Globals::get().shaders->singleColorShader) {}

void RenderBoundingBoxesStage::execute(gfx::Renderer& renderer, scene::Scene& scene,
                                       gfx::FrameBuffer*) {
    PROFILE_FUNCTION();

    setShaderTransforms(*scene.camera);
    prepareRenderer(renderer);

    auto [rigidBodies, transforms] =
        scene.ecsRegistry.getComponentsViews<RigidBodyComponent, TransformComponent>();

    m_boundingBoxShader->enable();

    for (auto& rigdyBody : rigidBodies) processRigidBody(rigdyBody, transforms, renderer, scene);

    m_boundingBoxShader->disable();

    renderer.restoreSettings();
}

void RenderBoundingBoxesStage::processRigidBody(RigidBodyComponent& rigidBody,
                                                TransformComponent::View& transforms,
                                                gfx::Renderer& renderer, scene::Scene& scene) {
    const auto& entityId = rigidBody.ownerEntityId;
    const auto& modelMatrix = utils::getModelMatrix(entityId, transforms);

    m_boundingBoxShader->setUniform("modelMatrix", modelMatrix);
    m_boundingBoxShader->setUniform("color", core::color::red);

    if (not rigidBody.renderBoundingBox || rigidBody.boundingBox == nullptr) return;

    physx::Vector velocityVector{modelMatrix * rigidBody.boundingBox->getCenterOfMass(),
                                 rigidBody.velocity};

    queueVelocityVectorForBeingRendered(std::move(velocityVector), scene);
    renderBoundingBox(renderer, *rigidBody.boundingBox);
}

void RenderBoundingBoxesStage::queueVelocityVectorForBeingRendered(physx::Vector&& velocityVector,
                                                                   scene::Scene& scene) {
    scene.vectors.emplace_back(physx::ColoredVector{std::move(velocityVector), core::color::green});
}

void RenderBoundingBoxesStage::setShaderTransforms(cam::Camera& camera) {
    m_boundingBoxShader->setUniform("projectionMatrix", camera.getProjectionMatrix());
    m_boundingBoxShader->setUniform("viewMatrix", camera.getViewMatrix());
}

void RenderBoundingBoxesStage::prepareRenderer(gfx::Renderer& renderer) {
    auto settings = renderer.getSettings();
    settings.polygonMode = STARL_LINE;
    renderer.setTemporarySettings(settings);
}

void RenderBoundingBoxesStage::renderBoundingBox(gfx::Renderer& renderer,
                                                 physx::BoundingBox& boundingBox) {
    auto vao = boundingBox.getVertexArray();

    vao->bind();
    renderer.renderVertexArray(*vao);
    vao->unbind();
}

}  // namespace sl::rendering::stages