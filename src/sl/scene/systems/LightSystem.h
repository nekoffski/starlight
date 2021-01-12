#pragma once

#include <memory>

#include "sl/ecs/ComponentView.hpp"
#include "sl/platform/shader/fwd.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::scene::systems {

class LightSystem {
public:
    void prepareDirectionalLights(ecs::ComponentView<components::DirectionalLightComponent>& lights,
        std::shared_ptr<graphics::Shader> shader);

    void preparePointsLights(ecs::ComponentView<components::PointLightComponent>& lights,
        ecs::ComponentView<components::TransformComponent>& transforms, std::shared_ptr<graphics::Shader> shader);

private:
    void setDirectionalLight(std::shared_ptr<graphics::Shader> shader,
        components::DirectionalLightComponent& light, unsigned int index);

    void setPointLight(std::shared_ptr<graphics::Shader> shader,
        components::PointLightComponent& light, components::TransformComponent& transform, unsigned int index);
};
}
