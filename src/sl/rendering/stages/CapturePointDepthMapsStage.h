#pragma once

#include "sl/rendering/CustomFrameBufferRenderPass.h"

#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"

#include "sl/scene/components/MeshRendererComponent.h"
#include "sl/scene/components/ModelComponent.h"
#include "sl/scene/components/PointLightComponent.h"
#include "sl/scene/components/TransformComponent.h"

namespace sl::rendering::stages {

class CapturePointDepthMapsStage : public Stage {
    inline static constexpr unsigned int facesCount = 6;

public:
    explicit CapturePointDepthMapsStage();

    void execute(gfx::LowLevelRenderer& renderer, scene::Scene& scene, gfx::FrameBuffer* frameBuffer) override;

private:
    void processLight(scene::components::PointLightComponent& light, scene::components::MeshRendererComponent::View& meshRenderers,
        scene::components::TransformComponent::View& transforms, scene::components::ModelComponent::View& models,
        gfx::LowLevelRenderer& renderer, gfx::FrameBuffer* frameBuffer);

    void setLightUniforms(const math::Vec3& lightPosition);

    void tryToRender(scene::components::MeshRendererComponent& meshRenderer, scene::components::TransformComponent::View& transforms,
        scene::components::ModelComponent::View& models, gfx::LowLevelRenderer& renderer);

    std::array<math::Mat4, facesCount> calculateShadowTransforms(const math::Vec3& lightPosition);

    void prepareRenderer(gfx::LowLevelRenderer& renderer);

    std::shared_ptr<gfx::Shader> m_depthShader;
    math::Mat4 m_shadowProjection;
};
}