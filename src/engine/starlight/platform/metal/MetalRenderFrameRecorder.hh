#pragma once

#include "Metal.hh"
#include "MetalRenderFrameLatch.hh"
#include "MetalResourcePool.hh"
#include "starlight/renderer/rhi/RenderFrameRecorder.hh"

namespace sl {

class MetalRenderFrameRecorder : public RenderFrameRecorder {
   public:
    struct AcquiredSurface {
        SurfaceHandle handle;
        CA::MetalDrawable* drawable;
    };

    explicit MetalRenderFrameRecorder(
        MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer
    );

    Result<RenderFrameSurfaceImage> acquireSurface(
        SurfaceHandle handle
    ) override;

    Result<void> renderPass(
        RenderPassCallback callback, const RenderPassDescription& description
    ) override;

    void schedulePresentations();

   private:
    MetalResourcePool& m_resourcePool;
    MTL::CommandBuffer* m_commandBuffer;
    std::vector<AcquiredSurface> m_acquiredSurfaces;
};

}  // namespace sl
