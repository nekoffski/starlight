#pragma once

#include <memory>

#include "sl/ecs/ComponentView.hpp"
#include "sl/gfx/Shader.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::gfx::renderer {

class LightRenderer {
public:
    void prepareDirectionalLights(ecs::ComponentView<scene::components::DirectionalLightComponent> lights,
        gfx::Shader& shader);

    void preparePointsLights(ecs::ComponentView<scene::components::PointLightComponent> lights,
        ecs::ComponentView<scene::components::TransformComponent> transforms, gfx::Shader& shader);

private:
    void setDirectionalLight(gfx::Shader& shader,
        const scene::components::DirectionalLightComponent& light, unsigned int index);

    void setPointLight(gfx::Shader& shader,
        const scene::components::PointLightComponent& light, const math::Mat4& transform, unsigned int index);
};
}
