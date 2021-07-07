#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class PrepareLightsStage : public Stage {
public:
    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::buffer::FrameBuffer*) override;

private:
    void prepareDirectionalLights(scene::components::DirectionalLightComponent::View& lights, gfx::Shader& shader);

    void preparePointLights(scene::components::PointLightComponent::View& lights,
        scene::components::TransformComponent::View& transforms, gfx::Shader& shader);

    void setDirectionalLightProperties(gfx::Shader& shader,
        const scene::components::DirectionalLightComponent& light, unsigned int index);

    void setPointLightProperties(gfx::Shader& shader, const scene::components::PointLightComponent& light,
        const math::Mat4& transform, unsigned int index);
};
}