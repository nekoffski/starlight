#pragma once

#include "RenderPass.hh"
#include "RenderResource.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Functional.hh"

namespace sl {

class RenderFrameRecorder : public NonCopyable, public NonMovable {
   public:
    using RenderPassCallback =
        MoveOnlyFunction<Result<void>(RenderPassEncoder&)>;

    virtual ~RenderFrameRecorder() = default;

    virtual Result<RenderFrameSurfaceImage> acquireSurface(
        SurfaceHandle handle
    ) = 0;

    virtual Result<void> renderPass(
        RenderPassCallback callback, const RenderPassDescription& description
    ) = 0;
};

}  // namespace sl
