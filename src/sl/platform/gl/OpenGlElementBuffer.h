#pragma once

#include "sl/gpu/ElementBuffer.h"

namespace sl::platform::gl {

class OpenGlElementBuffer : public gpu::ElementBuffer {
   public:
    OpenGlElementBuffer(void*, int, unsigned int);
    ~OpenGlElementBuffer() override;

    void bind() override;
    void unbind() override;

    unsigned int getIndicesCount() override { return m_indicesCount; }

   private:
    unsigned int m_indicesCount;
    unsigned int m_bufferId;
};

struct OpenGlElementBufferFactory : sl::gpu::ElementBuffer::Factory {
    std::shared_ptr<sl::gpu::ElementBuffer> create(void* data, int size, unsigned int indicesCount)
        override {
        return std::make_shared<OpenGlElementBuffer>(data, size, indicesCount);
    }
};
}  // namespace sl::platform::gl
