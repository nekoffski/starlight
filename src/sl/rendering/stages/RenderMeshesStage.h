#pragma once

#include "sl/gpu/Renderer.h"
#include "sl/rendering/DefaultFrameBufferRenderPass.h"
#include "sl/scene/components/MaterialComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class RenderMeshesStage : public Stage {
   public:
    void execute(gpu::Renderer& renderer, scene::Scene& scene, gpu::FrameBuffer*) override;

   private:
    void processMeshRendererComponent(
        scene::components::MeshRendererComponent& meshRendererComponent,
        scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models,
        scene::components::MaterialComponent::View& materials, gpu::Renderer& renderer,
        scene::Scene& scene
    );

    void prepareRenderer(
        const scene::components::MeshRendererComponent& meshRendererComponent,
        gpu::Renderer& renderer
    );

    const scene::components::MaterialComponent& getMaterial(
        const std::string& entityId, scene::components::MaterialComponent::View& materials
    );

    void setUniforms(
        gpu::Shader& shader, cam::Camera& camera,
        const scene::components::MaterialComponent& material
    );
    void setMaterialUniforms(
        const scene::components::MaterialComponent& material, gpu::Shader& shader
    );
};

}  // namespace sl::rendering::stages