#pragma once

#include "sl/graphics/buffer/VertexBuffer.h"

namespace sl::platform::gpu {

class OpenGlVertexBuffer : public graphics::buffer::VertexBuffer {
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

struct OpenGlVertexBufferFactory : graphics::buffer::VertexBuffer::Factory {
    std::shared_ptr<graphics::buffer::VertexBuffer> create(misc::types::NotNullPtr<void> data, int size, int verticesCount) override {
        return std::make_shared<OpenGlVertexBuffer>(data, size, verticesCount);
    }
};
}
