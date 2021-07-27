#pragma once

#include "sl/core/NotNullPtr.hpp"
#include "sl/gfx/buffer/ElementBuffer.h"

namespace sl::platform::gl {

class OpenGlElementBuffer : public sl::gfx::buffer::ElementBuffer {
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

struct OpenGlElementBufferFactory : sl::gfx::buffer::ElementBuffer::Factory {
    std::shared_ptr<sl::gfx::buffer::ElementBuffer> create(sl::core::NotNullPtr<void> data, int size, unsigned int indicesCount) override {
        return std::make_shared<OpenGlElementBuffer>(data, size, indicesCount);
    }
};
}
