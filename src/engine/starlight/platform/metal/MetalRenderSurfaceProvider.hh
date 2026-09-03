#pragma once

#include "Metal.hh"
#include "starlight/renderer/rhi/RenderSurfaceProvider.hh"

namespace sl {

class MetalRenderSurfaceProvider : public RenderSurfaceProvider {
   public:
    virtual CA::MetalLayer* createLayer() = 0;
    virtual void destroyLater(CA::MetalLayer* layer) = 0;
};

}  // namespace sl
