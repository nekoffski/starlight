#include "PrepareLightsStage.h"

#include "sl/core/Profiler.h"
#include "sl/rendering/utils/Misc.h"

#include "sl/core/Logger.h"

using namespace sl::scene::components;

namespace sl::rendering::stages {

void PrepareLightsStage::execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) {
    SL_PROFILE_FUNCTION();

    auto [rendererComponents, transforms, directionalLights, pointLights] =
        scene.ecsRegistry.getComponentsViews<MeshRendererComponent,
            TransformComponent, DirectionalLightComponent, PointLightComponent>();

    for (auto& rendererComponent : rendererComponents) {
        prepareDirectionalLights(directionalLights, *rendererComponent.shader);
        preparePointLights(pointLights, transforms, *rendererComponent.shader);
    }
}

void PrepareLightsStage::prepareDirectionalLights(DirectionalLightComponent::View& lights, gfx::Shader& shader) {
    unsigned int lightIndex = 0u;
    for (const auto& directionalLight : lights)
        if (directionalLight.isActive)
            setDirectionalLightProperties(shader, directionalLight, lightIndex++);

    shader.setUniform("directionalLightsCount", lightIndex);
}

void PrepareLightsStage::preparePointLights(PointLightComponent::View& lights, TransformComponent::View& transforms, gfx::Shader& shader) {
    unsigned int lightIndex = 0u;
    for (const auto& pointLight : lights)
        if (pointLight.isActive) {
            const auto& transform = utils::getModelMatrix(pointLight.ownerEntityId, transforms);
            setPointLightProperties(shader, pointLight, transform, lightIndex++);
        }

    shader.setUniform("pointLightsCount", lightIndex);
}

void PrepareLightsStage::setDirectionalLightProperties(gfx::Shader& shader, const DirectionalLightComponent& light, unsigned int index) {
    std::string strIndex = std::to_string(index);
    light.shadowMap->bind(index + 1);

    shader.setUniform("lightSpaceMatrix", light.spaceMatrix);
    shader.setUniform("directionalLights[" + strIndex + "].direction", light.direction);
    shader.setUniform("directionalLights[" + strIndex + "].color", light.color);
}

void PrepareLightsStage::setPointLightProperties(gfx::Shader& shader, const PointLightComponent& light, const math::Mat4& transform, unsigned int index) {
    std::string strIndex = std::to_string(index);

    light.omnidirectionalShadowMap->bind(3);

    shader.setUniform("farPlane", 25.0f);

    shader.setUniform("pointLights[" + strIndex + "].position", light.position);
    shader.setUniform("pointLights[" + strIndex + "].modelMatrix", transform);
    shader.setUniform("pointLights[" + strIndex + "].color", light.color);
    shader.setUniform("pointLights[" + strIndex + "].attenuationA", light.attenuationA);
    shader.setUniform("pointLights[" + strIndex + "].attenuationB", light.attenuationB);
    shader.setUniform("pointLights[" + strIndex + "].attenuationC", light.attenuationC);
}

}