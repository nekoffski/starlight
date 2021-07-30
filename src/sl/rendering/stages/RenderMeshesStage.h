#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

#include "sl/gfx/Renderer.h"

namespace sl::rendering::stages {

class RenderMeshesStage : public Stage {
public:
    void execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) override;

private:
    void processMeshRendererComponent(scene::components::MeshRendererComponent& meshRendererComponent, scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, scene::components::MaterialComponent::View& materials, gfx::Renderer& renderer, scene::Scene& scene);

    void prepareRenderer(const scene::components::MeshRendererComponent& meshRendererComponent, gfx::Renderer& renderer);

    const scene::components::MaterialComponent& getMaterial(const std::string& entityId, scene::components::MaterialComponent::View& materials);

    void setUniforms(gfx::Shader& shader, gfx::camera::Camera& camera, const scene::components::MaterialComponent& material);
    void setMaterialUniforms(const scene::components::MaterialComponent& material, gfx::Shader& shader);
};

}