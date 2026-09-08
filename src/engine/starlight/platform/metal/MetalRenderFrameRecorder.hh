#pragma once

#include "Metal.hh"
#include "MetalRenderFrameLatch.hh"
#include "MetalResourcePool.hh"
#include "starlight/renderer/rhi/RenderFrameRecorder.hh"

namespace sl {

class MetalRenderFrameRecorder : public RenderFrameRecorder {
   private:
    struct AcquiredSurface {
        SurfaceHandle handle;
        CA::MetalDrawable* drawable;
    };

   public:
    explicit MetalRenderFrameRecorder(
        MetalResourcePool& resourcePool, MTL::CommandBuffer* commandBuffer
    );

    Result<void> renderPass(
        RenderPassCallback callback, const RenderPassDescription& description
    ) override;

    void schedulePresentations();

   private:
    Result<CA::MetalDrawable*> acquireSurface(SurfaceHandle handle);
    Result<MTL::Texture*> resolveTarget(const RenderTarget& target);

    MetalResourcePool& m_resourcePool;
    MTL::CommandBuffer* m_commandBuffer;
    std::vector<AcquiredSurface> m_acquiredSurfaces;
};

}  // namespace sl
