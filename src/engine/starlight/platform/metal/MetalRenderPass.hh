#pragma once

#include "Metal.hh"
#include "MetalResourcePool.hh"
#include "starlight/renderer/rhi/RenderPass.hh"

namespace sl {

class MetalRenderPassEncoder : public RenderPassEncoder {
    struct MetalBufferSlice {
        MetalBuffer* buffer{nullptr};
        u32 offset{0};
    };

   public:
    explicit MetalRenderPassEncoder(
        MetalResourcePool& resourcePool, MTL::RenderCommandEncoder* encoder
    );
    ~MetalRenderPassEncoder() override;

    Result<void> setPipeline(GraphicsPipelineHandle pipeline) override;

    Result<void> setVertexBuffer(const DeviceBufferSlice& slice) override;
    Result<void> setIndexBuffer(const DeviceBufferSlice& slice) override;

    void draw(u32 vertexCount) override;
    void drawIndexed(u32 indexCount, u32 instanceCount = 1) override;

   private:
    MetalResourcePool& m_resourcePool;
    MTL::RenderCommandEncoder* m_encoder;
    Opt<MetalBufferSlice> m_indexBuffer;
};

MTL::LoadAction toMetal(LoadOp op);
MTL::StoreAction toMetal(StoreOp op);

}  // namespace sl