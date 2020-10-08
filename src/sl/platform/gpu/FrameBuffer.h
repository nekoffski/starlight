#pragma once

#include <memory>

#include "sl/platform/fwd.h"

namespace sl::platform::gpu {

class FrameBuffer {
public:
    static std::shared_ptr<FrameBuffer> create();

    virtual ~FrameBuffer() = default;

    virtual void bindTexture(std::shared_ptr<texture::Texture>) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
};
}