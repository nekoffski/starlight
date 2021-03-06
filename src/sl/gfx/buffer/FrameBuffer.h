#pragma once

#include <memory>

#include "sl/gfx/Cubemap.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/RenderBuffer.h"

namespace sl::gfx::buffer {

class FrameBuffer {
public:
    struct Factory {
        virtual std::shared_ptr<FrameBuffer> create() = 0;
        virtual ~Factory() = default;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual ~FrameBuffer() = default;

    virtual void bindRenderBuffer(RenderBuffer&) = 0;
    virtual void bindTexture(Texture&, unsigned int attachment) = 0;
    virtual void bindTexture(Texture&) = 0;
    virtual void bindCubemap(Cubemap&) = 0;

    virtual void specifyColorBuffers(const std::vector<unsigned int>&) = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;
};
}
