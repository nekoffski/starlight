#pragma once

#include "sl/gfx/buffer/FrameBuffer.h"

namespace sl::platform::gpu {

class OpenGlFrameBuffer : public gfx::buffer::FrameBuffer {
public:
    explicit OpenGlFrameBuffer();
    ~OpenGlFrameBuffer() override;

    void bind() override;
    void unbind() override;
    void bindTexture(std::shared_ptr<sl::gfx::Texture>) override;

private:
    unsigned int m_bufferId;
};

struct OpenGlFrameBufferFactory : gfx::buffer::FrameBuffer::Factory {
    std::shared_ptr<gfx::buffer::FrameBuffer> create() override {
        return std::make_shared<OpenGlFrameBuffer>();
    }
};
}
