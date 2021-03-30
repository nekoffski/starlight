
#include "LightRenderer.h"

#include "sl/core/Profiler.h"
#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/Shader.h"
#include "sl/scene/components/TransformComponent.h"

#include "glad/glad.h"

namespace sl::gfx::renderer {

void LightRenderer::prepareDirectionalLights(ecs::ComponentView<scene::components::DirectionalLightComponent> lights,
    std::shared_ptr<gfx::Shader> shader) {

    unsigned int directionalLightsNum = lights.size();
    shader->setUniform("directionalLightsNum", directionalLightsNum);

    for (unsigned int i = 0; i < directionalLightsNum; ++i)
        setDirectionalLight(shader, lights[i], i);
}

void LightRenderer::preparePointsLights(ecs::ComponentView<scene::components::PointLightComponent> lights,
    ecs::ComponentView<scene::components::TransformComponent> transforms, std::shared_ptr<gfx::Shader> shader) {

    unsigned int pointLightsNum = lights.size();
    shader->setUniform("pointLightsNum", pointLightsNum);

    for (unsigned int i = 0; i < pointLightsNum; ++i) {
        auto& transform = transforms.getByEntityId(lights[i].ownerEntityId);
        setPointLight(shader, lights[i], transform, i);
    }
}

void LightRenderer::setDirectionalLight(std::shared_ptr<gfx::Shader> shader,
    scene::components::DirectionalLightComponent& light, unsigned int index) {

    std::string strIndex = std::to_string(index);
    light.shadowMap->bind(1);

    shader->setUniform("lightSpaceMatrix", light.spaceMatrix);
    shader->setUniform("directionalLights[" + strIndex + "].direction", light.direction);
    shader->setUniform("directionalLights[" + strIndex + "].color", light.color);
}

void LightRenderer::setPointLight(std::shared_ptr<gfx::Shader> shader,
    scene::components::PointLightComponent& light, scene::components::TransformComponent& transform, unsigned int index) {

    std::string strIndex = std::to_string(index);

    shader->setUniform("pointLights[" + strIndex + "].position", light.position);
    shader->setUniform("pointLights[" + strIndex + "].modelMatrix", transform());
    shader->setUniform("pointLights[" + strIndex + "].color", light.color);
    shader->setUniform("pointLights[" + strIndex + "].attenuationA", light.attenuationA);
    shader->setUniform("pointLights[" + strIndex + "].attenuationB", light.attenuationB);
    shader->setUniform("pointLights[" + strIndex + "].attenuationC", light.attenuationC);
}
}
