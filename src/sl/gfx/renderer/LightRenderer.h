#pragma once

#include <memory>

#include "sl/ecs/ComponentView.hpp"
#include "sl/platform/shader/fwd.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gfx::renderer {

class LightRenderer {
public:
    void prepareDirectionalLights(ecs::ComponentView<scene::components::DirectionalLightComponent> lights,
        std::shared_ptr<gfx::Shader> shader);

    void preparePointsLights(ecs::ComponentView<scene::components::PointLightComponent> lights,
        ecs::ComponentView<scene::components::TransformComponent> transforms, std::shared_ptr<gfx::Shader> shader);

private:
    void setDirectionalLight(std::shared_ptr<gfx::Shader> shader,
        scene::components::DirectionalLightComponent& light, unsigned int index);

    void setPointLight(std::shared_ptr<gfx::Shader> shader,
        scene::components::PointLightComponent& light, scene::components::TransformComponent& transform, unsigned int index);
};
}
