#pragma once

#include "GeometryRenderData.h"
#include "CommandBuffer.h"

namespace sl {

struct RendererBackendProxy {
    virtual void drawGeometry(const GeometryRenderData& modelMatrix) = 0;
    virtual CommandBuffer* getCommandBuffer()                        = 0;
    virtual u32 getImageIndex()                                      = 0;
};

}  // namespace sl
