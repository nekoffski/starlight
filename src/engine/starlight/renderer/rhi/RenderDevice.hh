#pragma once

#include "RenderFrameRecorder.hh"
#include "RenderResource.hh"
#include "RenderSurfaceProvider.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"
#include "starlight/core/Functional.hh"

namespace sl {

class RenderDevice : public NonCopyable, public NonMovable {
   public:
    using RecordFrame = MoveOnlyFunction<Result<void>(RenderFrameRecorder&)>;

    virtual ~RenderDevice() = default;

    virtual Result<SurfaceHandle> attachSurface(
        std::shared_ptr<RenderSurfaceProvider> provider
    ) = 0;
    virtual void destroySurface(SurfaceHandle handle) = 0;

    virtual Result<void> trySubmitFrame(RecordFrame callback) = 0;
};

}  // namespace sl
