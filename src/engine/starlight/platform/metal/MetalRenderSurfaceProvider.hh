#pragma once

#include "Metal.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"

namespace sl {

class MetalRenderSurfaceProvider : public RenderSurfaceProvider {
   public:
    virtual CA::MetalLayer* getLayer() = 0;
};

}  // namespace sl
