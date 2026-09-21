#pragma once

#include "Metal.hh"
#include "MetalResourcePool.hh"
#include "starlight/renderer/rhi/RenderPass.hh"

namespace sl {

class MetalRenderPassEncoder : public RenderPassEncoder {
   public:
    explicit MetalRenderPassEncoder(
        MetalResourcePool& resourcePool, MTL::RenderCommandEncoder* encoder
    );
    ~MetalRenderPassEncoder() override;

    Result<void> setPipeline(GraphicsPipelineHandle pipeline) override;
    void draw(u32 vertexCount) override;

   private:
    MetalResourcePool& m_resourcePool;
    MTL::RenderCommandEncoder* m_encoder;
};

MTL::LoadAction toMetal(LoadOp op);
MTL::StoreAction toMetal(StoreOp op);

}  // namespace sl