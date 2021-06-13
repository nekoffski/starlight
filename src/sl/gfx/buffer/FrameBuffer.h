#pragma once

#include <memory>

#include "sl/gfx/Cubemap.h"
#include "sl/gfx/Texture.h"

namespace sl::gfx::buffer {

class FrameBuffer {
public:
    struct Factory {
        virtual std::shared_ptr<FrameBuffer> create() = 0;
        virtual ~Factory() = default;
    };

    inline static std::unique_ptr<Factory> factory = nullptr;

    virtual ~FrameBuffer() = default;

    virtual void bindTexture(sl::gfx::Texture&) = 0;
    virtual void bindCubemap(sl::gfx::Cubemap&) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
};
}
