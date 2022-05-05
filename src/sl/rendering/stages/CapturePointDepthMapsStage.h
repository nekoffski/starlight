#pragma once

#include "sl/gpu/Shader.h"
#include "sl/gpu/Texture.h"
#include "sl/rendering/CustomFrameBufferRenderPass.h"
#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class CapturePointDepthMapsStage : public Stage {
    inline static constexpr unsigned int facesCount = 6;

   public:
    explicit CapturePointDepthMapsStage();

    void execute(gpu::Renderer& renderer, scene::Scene& scene, gpu::FrameBuffer* frameBuffer)
        override;

   private:
    void processLight(
        scene::components::PointLightComponent& light,
        scene::components::MeshRendererComponent::View& meshRenderers,
        scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, gpu::Renderer& renderer,
        gpu::FrameBuffer* frameBuffer
    );

    void setLightUniforms(const math::Vec3& lightPosition);

    void tryToRender(
        scene::components::MeshRendererComponent& meshRenderer,
        scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, gpu::Renderer& renderer
    );

    std::array<math::Mat4, facesCount> calculateShadowTransforms(const math::Vec3& lightPosition);

    void prepareRenderer(gpu::Renderer& renderer);

    std::shared_ptr<gpu::Shader> m_depthShader;
    math::Mat4 m_shadowProjection;
};
}  // namespace sl::rendering::stages