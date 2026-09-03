#pragma once

#include "RenderResource.hh"
#include "RenderSurfaceProvider.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Core.hh"

namespace sl {

class RenderDevice : public NonCopyable, public NonMovable {
   public:
    virtual ~RenderDevice() = default;

    virtual Result<SurfaceHandle> createSurface(
        std::shared_ptr<RenderSurfaceProvider> provider
    ) = 0;
    virtual void destroySurface(SurfaceHandle handle) = 0;
};

}  // namespace sl
