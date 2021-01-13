
#include "LightSystem.h"

#include "sl/core/Profiler.h"
#include "sl/ecs/ComponentView.hpp"
#include "sl/graphics/Shader.h"
#include "sl/scene/components/TransformComponent.h"

#include "glad/glad.h"

namespace sl::scene::systems {

void LightSystem::prepareDirectionalLights(ecs::ComponentView<components::DirectionalLightComponent> lights,
    std::shared_ptr<graphics::Shader> shader) {

    unsigned int directionalLightsNum = lights.size();
    shader->setUniform("directionalLightsNum", directionalLightsNum);

    for (unsigned int i = 0; i < directionalLightsNum; ++i)
        setDirectionalLight(shader, lights[i], i);
}

void LightSystem::preparePointsLights(ecs::ComponentView<components::PointLightComponent> lights,
    ecs::ComponentView<components::TransformComponent> transforms, std::shared_ptr<graphics::Shader> shader) {

    unsigned int pointLightsNum = lights.size();
    shader->setUniform("pointLightsNum", pointLightsNum);

    for (unsigned int i = 0; i < pointLightsNum; ++i) {
        auto& transform = transforms.getByEntityId(lights[i].ownerEntityId);
        setPointLight(shader, lights[i], transform, i);
    }
}

void LightSystem::setDirectionalLight(std::shared_ptr<graphics::Shader> shader,
    components::DirectionalLightComponent& light, unsigned int index) {

    std::string strIndex = std::to_string(index);
    light.shadowMap->bind(1);

    shader->setUniform("lightSpaceMatrix", light.spaceMatrix);
    shader->setUniform("directionalLights[" + strIndex + "].direction", light.direction);
    shader->setUniform("directionalLights[" + strIndex + "].color", light.color);
}

void LightSystem::setPointLight(std::shared_ptr<graphics::Shader> shader,
    components::PointLightComponent& light, components::TransformComponent& transform, unsigned int index) {

    std::string strIndex = std::to_string(index);

    shader->setUniform("pointLights[" + strIndex + "].position", light.position);
    shader->setUniform("pointLights[" + strIndex + "].modelMatrix", transform());
    shader->setUniform("pointLights[" + strIndex + "].color", light.color);
    shader->setUniform("pointLights[" + strIndex + "].attenuationA", light.attenuationA);
    shader->setUniform("pointLights[" + strIndex + "].attenuationB", light.attenuationB);
    shader->setUniform("pointLights[" + strIndex + "].attenuationC", light.attenuationC);
}
}
