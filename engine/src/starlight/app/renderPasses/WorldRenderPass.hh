#pragma once

#include "starlight/renderer/RenderPass.hh"

namespace sl {

class WorldRenderPass : public RenderPass {
public:
    explicit WorldRenderPass(Renderer& renderer, const Vec4<f32>& viewportScale);

private:
    RenderPassBackend::Properties createRenderPassProperties(
      bool hasPreviousPass, bool hasNextPass
    ) override;

    void render(
      RenderPacket& packet, CommandBuffer& commandBuffer, u32 imageIndex,
      u64 frameNumber
    ) override;
};

}  // namespace sl
