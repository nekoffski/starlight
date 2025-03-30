#pragma once

#include "starlight/renderer/RenderPass.hh"

namespace sl {

class SkyboxRenderPass : public RenderPass {
public:
    explicit SkyboxRenderPass(Renderer& renderer, const Vec4<f32>& viewportScale);

private:
    RenderPassBackend::Properties createRenderPassProperties(
      bool hasPreviousPass, bool hasNextPass
    ) override;

    Pipeline::Properties createPipelineProperties() override;

    void render(
      RenderPacket& packet, CommandBuffer& commandBuffer, u32 imageIndex,
      u64 frameNumber
    ) override;
};

}  // namespace sl
