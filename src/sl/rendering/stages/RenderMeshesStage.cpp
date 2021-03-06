#include "RenderMeshesStage.h"

#include "sl/core/Profiler.h"
#include "sl/gfx/LowLevelRenderer.h"
#include "sl/rendering/utils/Mesh.h"
#include "sl/utils/Globals.h"

#include "sl/core/Profiler.h"
#include "sl/rendering/utils/Misc.h"

namespace sl::rendering::stages {

using namespace sl::scene::components;

void RenderMeshesStage::execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer* frameBuffer) {
    SL_PROFILE_FUNCTION();

    auto [meshRendererComponents, transforms, models, materials] =
        scene.ecsRegistry.getComponentsViews<MeshRendererComponent, TransformComponent, ModelComponent, MaterialComponent>();

    frameBuffer->specifyColorBuffers({ STARL_COLOR_ATTACHMENT0, STARL_COLOR_ATTACHMENT1 });

    for (auto& meshRendererComponent : meshRendererComponents)
        processMeshRendererComponent(meshRendererComponent, transforms, models, materials, renderer, scene);

    frameBuffer->specifyColorBuffers({ STARL_COLOR_ATTACHMENT0 });
}

void RenderMeshesStage::processMeshRendererComponent(MeshRendererComponent& meshRendererComponent, TransformComponent::View& transforms,
    ModelComponent::View& models, MaterialComponent::View& materials, gfx::LowLevelRenderer& renderer, scene::Scene& scene) {

    const auto& entityId = meshRendererComponent.ownerEntityId;

    if (not models.doesEntityOwnComponent(entityId))
        return;

    auto& model = models.getByEntityId(entityId);

    if (not model.isActive || not meshRendererComponent.isActive)
        return;

    auto& shader = *meshRendererComponent.shader;

    setUniforms(shader, *scene.camera, getMaterial(entityId, materials));
    prepareRenderer(meshRendererComponent, renderer);

    shader.enable();
    utils::renderModel(renderer, shader, model, utils::getModelMatrix(entityId, transforms));
    shader.disable();

    renderer.restoreSettings();
}

void RenderMeshesStage::prepareRenderer(const MeshRendererComponent& meshRendererComponent, gfx::LowLevelRenderer& renderer) {
    auto settings = renderer.getSettings();
    settings.polygonMode = meshRendererComponent.polygonMode;
    renderer.setTemporarySettings(settings);
}

const MaterialComponent& RenderMeshesStage::getMaterial(const std::string& entityId, MaterialComponent::View& materials) {
    static const MaterialComponent defaultMaterial {};
    return materials.doesEntityOwnComponent(entityId) ? materials.getByEntityId(entityId) : defaultMaterial;
}

void RenderMeshesStage::setUniforms(gfx::Shader& shader, gfx::camera::Camera& camera, const MaterialComponent& material) {
    shader.setUniform("viewMatrix", camera.getViewMatrix());
    shader.setUniform("projectionMatrix", camera.getProjectionMatrix());
    shader.setUniform("viewPosition", camera.getPosition());

    setMaterialUniforms(material, shader);
}

void RenderMeshesStage::setMaterialUniforms(const MaterialComponent& material, gfx::Shader& shader) {
    shader.setUniform("material.ambientColor", material.ambientColor);
    shader.setUniform("material.diffuseColor", material.diffuseColor);
    shader.setUniform("material.specularColor", material.specularColor);
    shader.setUniform("material.shininess", material.shininess);
}

}