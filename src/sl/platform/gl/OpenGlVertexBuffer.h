#pragma once

#include "sl/gfx/VertexBuffer.h"

namespace sl::platform::gl {

class OpenGlVertexBuffer : public sl::gfx::VertexBuffer {
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

struct OpenGlVertexBufferFactory : sl::gfx::VertexBuffer::Factory {
    std::shared_ptr<sl::gfx::VertexBuffer> create(void* data, int size, int verticesCount)
        override {
        return std::make_shared<OpenGlVertexBuffer>(data, size, verticesCount);
    }
};
}  // namespace sl::platform::gl
