#pragma once

#include "sl/glob/Globals.h"
#include "sl/rendering/DefaultFrameBufferRenderPass.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class RenderBoundingBoxesStage : public Stage {
   public:
    explicit RenderBoundingBoxesStage();

    void execute(gpu::Renderer& renderer, scene::Scene& scene, gpu::FrameBuffer*) override;

   private:
    void renderBoundingBox(gpu::Renderer& renderer, physx::BoundingBox& boundingBox);

    void setShaderTransforms(cam::Camera& camera);
    void prepareRenderer(gpu::Renderer& renderer);

    std::shared_ptr<gpu::Shader> m_boundingBoxShader;
};

}  // namespace sl::rendering::stages