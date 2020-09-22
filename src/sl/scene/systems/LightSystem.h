#pragma once

#include <memory>

#include "sl/platform/shader/fwd.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/PointLightComponent.h"

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
