#pragma once

#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/FrameBuffer.h"

namespace sl::platform::gfx {

class OpenGlFrameBuffer : public sl::gfx::buffer::FrameBuffer {
public:
    explicit OpenGlFrameBuffer();
    ~OpenGlFrameBuffer() override;

    void bind() override;
    void unbind() override;
    void bindTexture(sl::gfx::Texture&) override;

private:
    unsigned int m_bufferId;
};

struct OpenGlFrameBufferFactory : sl::gfx::buffer::FrameBuffer::Factory {
    std::shared_ptr<sl::gfx::buffer::FrameBuffer> create() override {
        return std::make_shared<OpenGlFrameBuffer>();
    }
};
}
