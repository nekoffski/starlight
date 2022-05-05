#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class PrepareLightsStage : public Stage {
   public:
    void execute(gpu::Renderer& renderer, scene::Scene& scene, gpu::FrameBuffer*) override;

   private:
    void prepareDirectionalLights(
        scene::components::DirectionalLightComponent::View& lights, gpu::Shader& shader
    );

    void preparePointLights(
        scene::components::PointLightComponent::View& lights,
        scene::components::TransformComponent::View& transforms, gpu::Shader& shader
    );

    void setDirectionalLightProperties(
        gpu::Shader& shader, const scene::components::DirectionalLightComponent& light,
        unsigned int index
    );

    void setPointLightProperties(
        gpu::Shader& shader, const scene::components::PointLightComponent& light,
        const math::Mat4& transform, unsigned int index
    );
};
}  // namespace sl::rendering::stages