#pragma once

#include "Metal.hh"
#include "starlight/renderer/rhi/RenderPass.hh"

namespace sl {

class MetalRenderPassEncoder : public RenderPassEncoder {
   public:
    explicit MetalRenderPassEncoder(MTL::RenderCommandEncoder* encoder);
    ~MetalRenderPassEncoder();

   private:
    MTL::RenderCommandEncoder* m_encoder;
};

MTL::LoadAction toMetal(LoadOp op);
MTL::StoreAction toMetal(StoreOp op);

}  // namespace sl