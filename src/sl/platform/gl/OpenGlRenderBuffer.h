#pragma once

#include "sl/gfx/RenderBuffer.h"

namespace sl::platform::gl {

class OpenGlRenderBuffer : public sl::gfx::RenderBuffer {
public:
    explicit OpenGlRenderBuffer(int format, unsigned int width, unsigned int height);

    ~OpenGlRenderBuffer() override;

    unsigned int getBufferId() const override;

    void bind() override;
    void unbind() override;

private:
    unsigned int m_buffferId;
};

struct OpenGlRenderBufferFactory : public sl::gfx::RenderBuffer::Factory {
    std::unique_ptr<sl::gfx::RenderBuffer> create(int format, unsigned int width, unsigned int height) override {
        return std::make_unique<OpenGlRenderBuffer>(format, width, height);
    }
};

}