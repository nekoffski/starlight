#pragma once

#include "sl/gfx/buffer/VertexBuffer.h"

namespace sl::platform::gpu {

class OpenGlVertexBuffer : public gfx::buffer::VertexBuffer {
public:
    explicit OpenGlVertexBuffer(void*, int, int);
    ~OpenGlVertexBuffer() override;

    void bind() override;
    void unbind() override;

    unsigned int getVerticesCount() override;

private:
    unsigned m_bufferId;
    unsigned m_verticesCount;
};

struct OpenGlVertexBufferFactory : gfx::buffer::VertexBuffer::Factory {
    std::shared_ptr<gfx::buffer::VertexBuffer> create(core::types::NotNullPtr<void> data, int size, int verticesCount) override {
        return std::make_shared<OpenGlVertexBuffer>(data, size, verticesCount);
    }
};
}
