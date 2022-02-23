#include "CapturePointDepthMapsStage.h"

#include <kc/core/Profiler.h>

#include "sl/gfx/ShaderManager.h"
#include "sl/rendering/utils/Mesh.h"
#include "sl/rendering/utils/Misc.h"

namespace sl::rendering::stages {

const float shadowWidth = gfx::Texture::shadowMapSize;
const float shadowHeight = shadowWidth;

const float aspect = shadowWidth / shadowHeight;

const float near = 1.0f;
const float far = 25.0f;

using namespace sl::scene::components;

CapturePointDepthMapsStage::CapturePointDepthMapsStage()
    : m_depthShader(gfx::ShaderManager::get().load(
          sl::glob::Globals::get().config.paths.shaders + "/PointDepthCapture.vert",
          sl::glob::Globals::get().config.paths.shaders + "/PointDepthCapture.frag",
          sl::glob::Globals::get().config.paths.shaders + "/PointDepthCapture.geom")) {
    m_shadowProjection = math::perspective(math::toRadians(90.0f), aspect, near, far);
}

void CapturePointDepthMapsStage::execute(gfx::Renderer& renderer, scene::Scene& scene,
                                         gfx::FrameBuffer* frameBuffer) {
    PROFILE_FUNCTION();

    prepareRenderer(renderer);

    m_depthShader->enable();
    m_depthShader->setUniform("farPlane", far);

    auto [meshRenderers, transforms, pointLights, models] =
        scene.ecsRegistry.getComponentsViews<MeshRendererComponent, TransformComponent,
                                             PointLightComponent, ModelComponent>();

    for (auto& light : pointLights) {
        if (not light.isActive) continue;

        processLight(light, meshRenderers, transforms, models, renderer, frameBuffer);
    }

    m_depthShader->disable();

    renderer.restoreSettings();
    renderer.restoreViewport();
}

void CapturePointDepthMapsStage::processLight(PointLightComponent& light,
                                              MeshRendererComponent::View& meshRenderers,
                                              TransformComponent::View& transforms,
                                              ModelComponent::View& models, gfx::Renderer& renderer,
                                              gfx::FrameBuffer* frameBuffer) {
    const auto& transform = utils::getModelMatrix(light.ownerEntityId, transforms);
    setLightUniforms(transform * light.position);

    frameBuffer->bindCubemap(*light.omnidirectionalShadowMap);

    renderer.clearBuffers(STARL_DEPTH_BUFFER_BIT);

    for (auto& meshRenderer : meshRenderers)
        if (not meshRenderer.isTransparentForLight)
            tryToRender(meshRenderer, transforms, models, renderer);
}

void CapturePointDepthMapsStage::setLightUniforms(const math::Vec3& lightPosition) {
    auto shadowTransforms = calculateShadowTransforms(lightPosition);

    for (int i = 0; i < facesCount; ++i)
        m_depthShader->setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

    m_depthShader->setUniform("lightPosition", lightPosition);
}

void CapturePointDepthMapsStage::tryToRender(MeshRendererComponent& meshRenderer,
                                             TransformComponent::View& transforms,
                                             ModelComponent::View& models,
                                             gfx::Renderer& renderer) {
    const auto& entityId = meshRenderer.ownerEntityId;

    if (not models.doesEntityOwnComponent(entityId)) return;

    utils::renderModel(renderer, *m_depthShader, models.getByEntityId(entityId),
                       utils::getModelMatrix(entityId, transforms));
}

std::array<math::Mat4, 6> CapturePointDepthMapsStage::calculateShadowTransforms(
    const math::Vec3& lightPosition) {
    std::array<math::Mat4, 6> shadowTransforms;

    shadowTransforms[0] =
        m_shadowProjection * math::lookAt(lightPosition, lightPosition + math::Vec3(1.0, 0.0, 0.0),
                                          math::Vec3(0.0, -1.0, 0.0));

    shadowTransforms[1] =
        m_shadowProjection * math::lookAt(lightPosition, lightPosition + math::Vec3(-1.0, 0.0, 0.0),
                                          math::Vec3(0.0, -1.0, 0.0));

    shadowTransforms[2] =
        m_shadowProjection * math::lookAt(lightPosition, lightPosition + math::Vec3(0.0, 1.0, 0.0),
                                          math::Vec3(0.0, 0.0, 1.0));

    shadowTransforms[3] =
        m_shadowProjection * math::lookAt(lightPosition, lightPosition + math::Vec3(0.0, -1.0, 0.0),
                                          math::Vec3(0.0, 0.0, -1.0));

    shadowTransforms[4] =
        m_shadowProjection * math::lookAt(lightPosition, lightPosition + math::Vec3(0.0, 0.0, 1.0),
                                          math::Vec3(0.0, -1.0, 0.0));

    shadowTransforms[5] =
        m_shadowProjection * math::lookAt(lightPosition, lightPosition + math::Vec3(0.0, 0.0, -1.0),
                                          math::Vec3(0.0, -1.0, 0.0));

    return shadowTransforms;
}

void CapturePointDepthMapsStage::prepareRenderer(gfx::Renderer& renderer) {
    renderer.setTemporaryViewport(
        gfx::ViewFrustum::Viewport{gfx::Texture::shadowMapSize, gfx::Texture::shadowMapSize});

    auto settings = renderer.getSettings();
    settings.cullFace = STARL_FRONT;
    renderer.setTemporarySettings(settings);
}
}  // namespace sl::rendering::stages