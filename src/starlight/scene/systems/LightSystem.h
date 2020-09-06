#pragma once

#include <memory>

#include "starlight/platform/shader/fwd.h"
#include "starlight/scene/components/DirectionalLightComponent.h"
#include "starlight/scene/components/PointLightComponent.h"

namespace sl::scene::systems {

class LightSystem {
public:
    void prepareDirectionalLights(std::vector<components::DirectionalLightComponent>& lights,
        std::shared_ptr<platform::shader::Shader> shader);
    void preparePointsLights(std::vector<components::PointLightComponent>& lights,
        std::shared_ptr<platform::shader::Shader> shader);

private:
    void setDirectionalLight(std::shared_ptr<platform::shader::Shader> shader,
        components::DirectionalLightComponent light, unsigned int index);
    void setPointLight(std::shared_ptr<platform::shader::Shader> shader,
        components::PointLightComponent light, unsigned int index);
};
}
