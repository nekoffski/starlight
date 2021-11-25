#pragma once

#include "sl/gfx/ElementBuffer.h"

namespace sl::platform::gl {

class OpenGlElementBuffer : public gfx::ElementBuffer {
public:
    OpenGlElementBuffer(void*, int, unsigned int);
    ~OpenGlElementBuffer() override;

    void bind() override;
    void unbind() override;

    unsigned int getIndicesCount() override {
        return m_indicesCount;
    }

private:
    unsigned int m_indicesCount;
    unsigned int m_bufferId;
};

struct OpenGlElementBufferFactory : sl::gfx::ElementBuffer::Factory {
    std::shared_ptr<sl::gfx::ElementBuffer> create(void* data, int size, unsigned int indicesCount) override {
        return std::make_shared<OpenGlElementBuffer>(data, size, indicesCount);
    }
};
}
