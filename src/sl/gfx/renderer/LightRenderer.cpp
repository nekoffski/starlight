
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
    for (auto& directionalLight : lights)
        if (directionalLight.isActive)
            setDirectionalLight(shader, directionalLight, lightIndex++);

    shader.setUniform("directionalLightsNum", lightIndex);
}

void LightRenderer::preparePointsLights(ecs::ComponentView<scene::components::PointLightComponent> lights,
    ecs::ComponentView<scene::components::TransformComponent> transforms, Shader& shader) {

    unsigned int lightIndex = 0;
    for (auto& pointLight : lights)
        if (pointLight.isActive)
            setPointLight(shader, pointLight,
                transforms.getByEntityId(pointLight.ownerEntityId), lightIndex++);

    shader.setUniform("pointLightsNum", lightIndex);
}

void LightRenderer::setDirectionalLight(Shader& shader, scene::components::DirectionalLightComponent& light,
    unsigned int index) {

    std::string strIndex = std::to_string(index);
    light.shadowMap->bind(index + 1);

    shader.setUniform("lightSpaceMatrix", light.spaceMatrix);
    shader.setUniform("directionalLights[" + strIndex + "].direction", light.direction);
    shader.setUniform("directionalLights[" + strIndex + "].color", light.color);
}

void LightRenderer::setPointLight(Shader& shader, scene::components::PointLightComponent& light,
    scene::components::TransformComponent& transform, unsigned int index) {

    std::string strIndex = std::to_string(index);

    shader.setUniform("pointLights[" + strIndex + "].position", light.position);
    shader.setUniform("pointLights[" + strIndex + "].modelMatrix", transform.transformation);
    shader.setUniform("pointLights[" + strIndex + "].color", light.color);
    shader.setUniform("pointLights[" + strIndex + "].attenuationA", light.attenuationA);
    shader.setUniform("pointLights[" + strIndex + "].attenuationB", light.attenuationB);
    shader.setUniform("pointLights[" + strIndex + "].attenuationC", light.attenuationC);
}
}
