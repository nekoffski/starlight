#pragma once

#include "sl/gfx/buffer/RenderBuffer.h"

namespace sl::platform::gfx {

class OpenGlRenderBuffer : public sl::gfx::buffer::RenderBuffer {
public:
    explicit OpenGlRenderBuffer(int format, unsigned int width, unsigned int height);

    ~OpenGlRenderBuffer() override;

    unsigned int getBufferId() const override;

    void bind() override;
    void unbind() override;

private:
    unsigned int m_buffferId;
};

struct OpenGlRenderBufferFactory : public sl::gfx::buffer::RenderBuffer::Factory {
    std::unique_ptr<sl::gfx::buffer::RenderBuffer> create(int format, unsigned int width, unsigned int height) override {
        return std::make_unique<OpenGlRenderBuffer>(format, width, height);
    }
};

}