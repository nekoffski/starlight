#pragma once

#include "Metal.hh"
#include "MetalRenderFrameLatch.hh"
#include "MetalResourcePool.hh"
#include "starlight/renderer/rhi/RenderFrameRecorder.hh"

namespace sl {

class MetalRenderFrameRecorder : public RenderFrameRecorder {
   public:
    explicit MetalRenderFrameRecorder(
        MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer
    );

   private:
    MetalResourcePool& m_resourcePool;
    MTL::CommandBuffer* m_commandBuffer;
};

}  // namespace sl
