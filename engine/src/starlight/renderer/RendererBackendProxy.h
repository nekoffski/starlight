#pragma once

#include "GeometryRenderData.h"
#include "CommandBuffer.h"
#include "Texture.h"

namespace sl {

struct RendererBackendProxy {
    virtual void drawGeometry(const Geometry& geometry) = 0;
    virtual CommandBuffer* getCommandBuffer()           = 0;
    virtual u32 getImageIndex()                         = 0;
    virtual Texture* getFramebuffer(u64 id)             = 0;
    virtual Texture* getDepthBuffer()                   = 0;
};

}  // namespace sl
