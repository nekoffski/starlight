#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

#include "sl/gfx/LowLevelRenderer.h"

namespace sl::rendering::stages {

class RenderMeshesStage : public DefaultFrameBufferRenderPass::Stage {
public:
    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) override;

private:
    void processMeshRendererComponent(scene::components::MeshRendererComponent& meshRendererComponent, scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, scene::components::MaterialComponent::View& materials, gfx::LowLevelRenderer& renderer, scene::Scene& scene);

    void prepareRenderer(const scene::components::MeshRendererComponent& meshRendererComponent, gfx::LowLevelRenderer& renderer);

    const scene::components::MaterialComponent& getMaterial(const std::string& entityId, scene::components::MaterialComponent::View& materials);

    void setShaderUniforms(gfx::Shader& shader, gfx::camera::Camera& camera, const scene::components::MaterialComponent& material);
    void setMaterialUniforms(const scene::components::MaterialComponent& material, gfx::Shader& shader);
};

}