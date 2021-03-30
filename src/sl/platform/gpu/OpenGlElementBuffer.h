#pragma once

#include "sl/gfx/buffer/ElementBuffer.h"
#include "sl/core/types/NotNullPtr.hpp"

namespace sl::platform::gpu {

class OpenGlElementBuffer : public gfx::buffer::ElementBuffer {
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

struct OpenGlElementBufferFactory : gfx::buffer::ElementBuffer::Factory {
    std::shared_ptr<gfx::buffer::ElementBuffer> create(core::types::NotNullPtr<void> data, int size, unsigned int indicesCount) override {
        return std::make_shared<OpenGlElementBuffer>(data, size, indicesCount);
    }
};
}
