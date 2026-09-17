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
    ~MetalRenderPassEncoder();

    Result<void> setPipeline(GraphicsPipelineHandle pipeline);
    void draw(u32 vertexCount);

   private:
    MetalResourcePool& m_resourcePool;
    MTL::RenderCommandEncoder* m_encoder;
};

MTL::LoadAction toMetal(LoadOp op);
MTL::StoreAction toMetal(StoreOp op);

}  // namespace sl