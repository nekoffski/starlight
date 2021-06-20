#pragma once

#include "sl/rendering/DefaultFrameBufferRenderPass.h"

#include "sl/scene/components/RigidBodyComponent.h"
#include "sl/scene/components/TransformComponent.h"
#include "sl/utils/Globals.h"

namespace sl::rendering::stages {

class RenderBoundingBoxesStage : public DefaultFrameBufferRenderPass::Stage {
public:
    explicit RenderBoundingBoxesStage();

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene) override;

private:
    void renderBoundingBox(gfx::LowLevelRenderer& renderer, physx::BoundingBox& boundingBox);

    void setShaderTransforms(gfx::camera::Camera& camera);
    void prepareRenderer(gfx::LowLevelRenderer& renderer);

    void processRigidBody(scene::components::RigidBodyComponent& rigidBody, scene::components::TransformComponent::View& transforms,
        gfx::LowLevelRenderer& renderer, scene::Scene& scene);

    void queueVelocityVectorForBeingRendered(physx::Vector&& velocityVector, scene::Scene& scene);

    std::shared_ptr<gfx::Shader> m_boundingBoxShader;
};

}