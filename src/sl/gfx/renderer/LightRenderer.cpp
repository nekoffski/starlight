
#include "LightRenderer.h"

#include "sl/core/Profiler.h"
#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/Shader.h"
#include "sl/scene/components/TransformComponent.h"

#include "glad/glad.h"

namespace sl::gfx::renderer {

void LightRenderer::prepareDirectionalLights(ecs::ComponentView<scene::components::DirectionalLightComponent> lights,
    gfx::Shader& shader) {

    unsigned int lightIndex = 0;
    for (const auto& directionalLight : lights)
        if (directionalLight.isActive)
            setDirectionalLight(shader, directionalLight, lightIndex++);

    shader.setUniform("directionalLightsNum", lightIndex);
}

void LightRenderer::preparePointsLights(ecs::ComponentView<scene::components::PointLightComponent> lights,
    ecs::ComponentView<scene::components::TransformComponent> transforms, Shader& shader) {

    unsigned int lightIndex = 0;
    for (const auto& pointLight : lights)
        if (pointLight.isActive) {
            auto& transform = transforms.doesEntityOwnComponent(pointLight.ownerEntityId)
                ? transforms.getByEntityId(pointLight.ownerEntityId).transformation
                : math::identityMatrix;

            setPointLight(shader, pointLight, transform, lightIndex++);
        }

    shader.setUniform("pointLightsNum", lightIndex);
}

void LightRenderer::setDirectionalLight(Shader& shader, const scene::components::DirectionalLightComponent& light,
    unsigned int index) {

    std::string strIndex = std::to_string(index);
    light.shadowMap->bind(index + 1);

    shader.setUniform("lightSpaceMatrix", light.spaceMatrix);
    shader.setUniform("directionalLights[" + strIndex + "].direction", light.direction);
    shader.setUniform("directionalLights[" + strIndex + "].color", light.color);
}

void LightRenderer::setPointLight(Shader& shader, const scene::components::PointLightComponent& light, const math::Mat4& transform, unsigned int index) {

    std::string strIndex = std::to_string(index);

    shader.setUniform("pointLights[" + strIndex + "].position", light.position);
    shader.setUniform("pointLights[" + strIndex + "].modelMatrix", transform);
    shader.setUniform("pointLights[" + strIndex + "].color", light.color);
    shader.setUniform("pointLights[" + strIndex + "].attenuationA", light.attenuationA);
    shader.setUniform("pointLights[" + strIndex + "].attenuationB", light.attenuationB);
    shader.setUniform("pointLights[" + strIndex + "].attenuationC", light.attenuationC);
}
}
