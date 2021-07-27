#pragma once

#include "sl/gfx/buffer/VertexBuffer.h"

namespace sl::platform::gl {

class OpenGlVertexBuffer : public sl::gfx::buffer::VertexBuffer {
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

struct OpenGlVertexBufferFactory : sl::gfx::buffer::VertexBuffer::Factory {
    std::shared_ptr<sl::gfx::buffer::VertexBuffer> create(sl::core::NotNullPtr<void> data, int size, int verticesCount) override {
        return std::make_shared<OpenGlVertexBuffer>(data, size, verticesCount);
    }
};
}
