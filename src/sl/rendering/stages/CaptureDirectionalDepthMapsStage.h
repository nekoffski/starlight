#pragma once

#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/VertexArray.h"
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

    void execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer* frameBuffer)
        override;

   private:
    void renderDepth(
        scene::components::DirectionalLightComponent& light,
        scene::components::MeshRendererComponent::View& meshRenderers,
        scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, gfx::Renderer& renderer
    );

    void tryToRenderModel(
        scene::components::MeshRendererComponent& meshRenderer,
        scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, gfx::Renderer& renderer
    );

    void queueDirectionVectorForBeingRendered(const math::Vec3& direction, scene::Scene& scene);

    void prepareRenderer(gfx::Renderer& renderer);

    std::shared_ptr<gfx::Shader> m_depthShader;
};

}  // namespace sl::rendering::stages