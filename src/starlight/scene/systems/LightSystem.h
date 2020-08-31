#pragma once

#include <memory>

#include "starlight/core/perf/Profiler.hpp"
#include "starlight/platform/shader/Shader.h"
#include "starlight/scene/components/DirectionalLightComponent.h"
#include "starlight/scene/components/PointLightComponent.h"
#include "starlight/scene/components/TransformComponent.h"

namespace sl::scene::systems {

class LightSystem {
public:
    void prepareDirectionalLights(std::vector<components::DirectionalLightComponent>& lights,
        std::shared_ptr<platform::shader::Shader> shader) {
        for (const auto& light : lights) {
            shader->setUniform("directionalLights[0].direction", light.direction);
            shader->setUniform("directionalLights[0].color", light.color);
        }
    }

    void preparePointsLights(std::vector<components::PointLightComponent>& lights,
        std::shared_ptr<platform::shader::Shader> shader) {
        for (const auto& light : lights) {
            auto& transform = light.entity->getComponent<components::TransformComponent>();
            // TODO: optimize
            shader->setUniform("pointLights[0].position", light.position);
            shader->setUniform("pointLights[0].modelMatrix", transform());
            shader->setUniform("pointLights[0].color", light.color);
            shader->setUniform("pointLights[0].attenuationA", light.attenuationA);
            shader->setUniform("pointLights[0].attenuationB", light.attenuationB);
            shader->setUniform("pointLights[0].attenuationC", light.attenuationC);
        }
    }
};
}
