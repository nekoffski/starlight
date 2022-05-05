#pragma once

#include "sl/gpu/Shader.h"
#include "sl/gpu/Texture.h"
#include "sl/gpu/VertexArray.h"
#include "sl/rendering/CustomFrameBufferRenderPass.h"
#include "sl/rendering/utils/Mesh.h"
#include "sl/rendering/utils/Misc.h"
#include "sl/scene/components/DirectionalLightComponent.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class CaptureDirectionalDepthMapsStage : public Stage {
   public:
    explicit CaptureDirectionalDepthMapsStage();

    void execute(gpu::Renderer& renderer, scene::Scene& scene, gpu::FrameBuffer* frameBuffer)
        override;

   private:
    void renderDepth(
        scene::components::DirectionalLightComponent& light,
        scene::components::MeshRendererComponent::View& meshRenderers,
        scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, gpu::Renderer& renderer
    );

    void tryToRenderModel(
        scene::components::MeshRendererComponent& meshRenderer,
        scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, gpu::Renderer& renderer
    );

    void queueDirectionVectorForBeingRendered(const math::Vec3& direction, scene::Scene& scene);

    void prepareRenderer(gpu::Renderer& renderer);

    std::shared_ptr<gpu::Shader> m_depthShader;
};

}  // namespace sl::rendering::stages