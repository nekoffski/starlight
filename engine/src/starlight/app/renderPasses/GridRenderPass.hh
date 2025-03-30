#pragma once

#include "starlight/renderer/RenderPass.hh"

namespace sl {

class GridRenderPass : public RenderPass {
public:
    explicit GridRenderPass(Renderer& renderer, const Vec4<f32>& viewportScale);

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
