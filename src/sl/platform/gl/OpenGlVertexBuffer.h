#pragma once

#include "sl/gpu/VertexBuffer.h"

namespace sl::platform::gl {

class OpenGlVertexBuffer : public sl::gpu::VertexBuffer {
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

struct OpenGlVertexBufferFactory : sl::gpu::VertexBuffer::Factory {
    std::shared_ptr<sl::gpu::VertexBuffer> create(void* data, int size, int verticesCount)
        override {
        return std::make_shared<OpenGlVertexBuffer>(data, size, verticesCount);
    }
};
}  // namespace sl::platform::gl
