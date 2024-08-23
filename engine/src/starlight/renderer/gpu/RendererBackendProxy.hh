#pragma once

#include "CommandBuffer.hh"
#include "Texture.hh"
#include "Mesh.hh"

#include "starlight/core/math/Core.hh"
#include "starlight/core/memory/Memory.hh"

#include "UIRenderer.hh"

namespace sl {

struct RendererBackendProxy {
    virtual void drawMesh(const Mesh& mesh)                                = 0;
    virtual CommandBuffer* getCommandBuffer()                              = 0;
    virtual u32 getImageIndex()                                            = 0;
    virtual Texture* getFramebuffer(u64 id)                                = 0;
    virtual Texture* getDepthBuffer()                                      = 0;
    virtual void gpuCall(std::function<void(CommandBuffer&)>&&)            = 0;
    virtual UniqPtr<UIRenderer> createUIRendererer(RenderPass* renderPass) = 0;
    virtual void setViewport(const Rect2<u32>& viewport)                   = 0;
};

}  // namespace sl
