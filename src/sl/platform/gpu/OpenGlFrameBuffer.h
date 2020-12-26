#pragma once

#include "sl/graphics/buffer/FrameBuffer.h"

namespace sl::platform::gpu {

class OpenGlFrameBuffer : public graphics::buffer::FrameBuffer {
public:
    explicit OpenGlFrameBuffer();
    ~OpenGlFrameBuffer() override;

    void bind() override;
    void unbind() override;
    void bindTexture(std::shared_ptr<sl::graphics::Texture>) override;

private:
    unsigned int m_bufferId;
};

struct OpenGlFrameBufferFactory : graphics::buffer::FrameBuffer::Factory {
    std::shared_ptr<graphics::buffer::FrameBuffer> create() override {
        return std::make_shared<OpenGlFrameBuffer>();
    }
};
}
