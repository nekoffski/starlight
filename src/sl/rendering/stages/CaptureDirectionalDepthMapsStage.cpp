#include "CaptureDirectionalDepthMapsStage.h"

#include "sl/core/Profiler.h"
#include "sl/rendering/utils/Mesh.h"
#include "sl/rendering/utils/Misc.h"
#include "sl/utils/Globals.h"

namespace sl::rendering::stages {

using namespace sl::scene::components;

CaptureDirectionalDepthMapsStage::CaptureDirectionalDepthMapsStage()
    : m_depthShader(gfx::Shader::load(
          GLOBALS().config.paths.shaders + "/depth_capture.vert", GLOBALS().config.paths.shaders + "/depth_capture.frag")) {
}

void CaptureDirectionalDepthMapsStage::execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer& frameBuffer) {
    SL_PROFILE_FUNCTION();

    prepareRenderer(renderer);

    auto [meshRenderers, transforms, directionalLights, models] =
        scene.ecsRegistry.getComponentsViews<MeshRendererComponent, TransformComponent, DirectionalLightComponent, ModelComponent>();

    m_depthShader->enable();

    for (auto& light : directionalLights) {
        if (not light.isActive)
            continue;

        if (light.renderDirection)
            queueDirectionVectorForBeingRendered(light.direction, scene);

        frameBuffer.bindTexture(*light.shadowMap);

        renderDepth(light, meshRenderers, transforms, models, renderer);
    }

    m_depthShader->disable();

    renderer.restoreSettings();
    renderer.restoreViewport();
}

void CaptureDirectionalDepthMapsStage::renderDepth(DirectionalLightComponent& light, MeshRendererComponent::View& meshRenderers,
    TransformComponent::View& transforms, ModelComponent::View& models, gfx::LowLevelRenderer& renderer) {

    m_depthShader->setUniform("lightSpaceMatrix", light.spaceMatrix);

    for (auto& meshRenderer : meshRenderers)
        tryToRenderModel(meshRenderer, transforms, models, renderer);
}

void CaptureDirectionalDepthMapsStage::tryToRenderModel(MeshRendererComponent& meshRenderer, TransformComponent::View& transforms,
    ModelComponent::View& models, gfx::LowLevelRenderer& renderer) {

    const auto& entityId = meshRenderer.ownerEntityId;

    if (not models.doesEntityOwnComponent(entityId))
        return;

    utils::renderModel(renderer, *m_depthShader, models.getByEntityId(entityId),
        utils::getModelMatrix(entityId, transforms));
}

void CaptureDirectionalDepthMapsStage::queueDirectionVectorForBeingRendered(const math::Vec3& direction, scene::Scene& scene) {
    static const math::Vec3 sceneOrigin { 0.0f, 0.0f, 0.0f };
    static constexpr float directionScale = 1000.0f;

    physx::Vector vector {
        sceneOrigin, direction * directionScale
    };
    scene.vectors.emplace_back(physx::ColoredVector { std::move(vector), core::color::blue });
}

void CaptureDirectionalDepthMapsStage::prepareRenderer(gfx::LowLevelRenderer& renderer) {
    renderer.setTemporaryViewport(gfx::ViewFrustum::Viewport { gfx::Texture::shadowMapSize, gfx::Texture::shadowMapSize });

    auto settings = renderer.getSettings();
    settings.cullFace = STARL_FRONT;
    renderer.setTemporarySettings(settings);
}
}