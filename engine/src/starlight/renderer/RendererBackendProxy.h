#pragma once

#include "CommandBuffer.h"
#include "Texture.h"
#include "Geometry.h"

#include "starlight/core/memory/Memory.hpp"

#include "ui/UIRenderer.h"

namespace sl {

struct RendererBackendProxy {
    virtual void drawGeometry(const Geometry& geometry)                    = 0;
    virtual CommandBuffer* getCommandBuffer()                              = 0;
    virtual u32 getImageIndex()                                            = 0;
    virtual Texture* getFramebuffer(u64 id)                                = 0;
    virtual Texture* getDepthBuffer()                                      = 0;
    virtual void gpuCall(std::function<void(CommandBuffer&)>&&)            = 0;
    virtual UniqPtr<UIRenderer> createUIRendererer(RenderPass* renderPass) = 0;
};

}  // namespace sl
