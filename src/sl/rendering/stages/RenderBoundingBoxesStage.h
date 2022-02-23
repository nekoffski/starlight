#pragma once

#include "sl/glob/Globals.h"
#include "sl/rendering/DefaultFrameBufferRenderPass.h"
#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class RenderBoundingBoxesStage : public Stage {
   public:
    explicit RenderBoundingBoxesStage();

    void execute(gfx::Renderer& renderer, scene::Scene& scene, gfx::FrameBuffer*) override;

   private:
    void renderBoundingBox(gfx::Renderer& renderer, physx::BoundingBox& boundingBox);

    void setShaderTransforms(cam::Camera& camera);
    void prepareRenderer(gfx::Renderer& renderer);

    void processRigidBody(scene::components::RigidBodyComponent& rigidBody,
                          scene::components::TransformComponent::View& transforms,
                          gfx::Renderer& renderer, scene::Scene& scene);

    void queueVelocityVectorForBeingRendered(physx::Vector&& velocityVector, scene::Scene& scene);

    std::shared_ptr<gfx::Shader> m_boundingBoxShader;
};

}  // namespace sl::rendering::stages