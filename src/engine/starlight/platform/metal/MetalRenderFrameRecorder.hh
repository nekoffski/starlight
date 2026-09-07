#pragma once

#include "Metal.hh"
#include "MetalRenderFrameFence.hh"
#include "MetalResourcePool.hh"
#include "starlight/renderer/rhi/RenderFrameRecorder.hh"

namespace sl {

class MetalRenderFrameRecorder : public RenderFrameRecorder {
   public:
    explicit MetalRenderFrameRecorder(
        MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer,
        u8 frameSlot
    );

   private:
    MetalResourcePool& m_resourcePool;
    MTL::CommandBuffer* m_commandBuffer;
    u8 m_frameSlot;
};

}  // namespace sl
