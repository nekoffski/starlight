#pragma once

#include "starlight/platform/gpu/ElementBuffer.h"

namespace sl::platform::gpu::opengl {

class OpenGLElementBuffer : public ElementBuffer {
public:
    OpenGLElementBuffer(void*, int, unsigned int);
    ~OpenGLElementBuffer() override;

    void bind() override;
    void unbind() override;

    unsigned int getIndicesCount() override {
        return m_indicesCount;
    }

private:
    unsigned int m_indicesCount;
    unsigned int m_bufferId;
};
}