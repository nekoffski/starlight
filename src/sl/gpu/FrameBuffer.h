#pragma once

#include <memory>

#include "sl/gpu/Cubemap.h"
#include "sl/gpu/RenderBuffer.h"
#include "sl/gpu/Texture.h"

namespace sl::gpu {

class FrameBuffer {
   public:
    struct Factory {
        virtual std::shared_ptr<FrameBuffer> create() = 0;
        virtual ~Factory()                            = default;
    };

    virtual ~FrameBuffer() = default;

    virtual void bindRenderBuffer(RenderBuffer&)                = 0;
    virtual void bindTexture(Texture&, unsigned int attachment) = 0;
    virtual void bindTexture(Texture&)                          = 0;
    virtual void bindCubemap(Cubemap&)                          = 0;

    virtual void specifyColorBuffers(const std::vector<unsigned int>&) = 0;

    virtual void bind()   = 0;
    virtual void unbind() = 0;
};
}  // namespace sl::gpu
