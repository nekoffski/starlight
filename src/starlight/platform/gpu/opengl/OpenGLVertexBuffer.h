#pragma once

#include <starlight/platform/gpu/VertexBuffer.h>

namespace starl::platform::gpu::opengl {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    explicit OpenGLVertexBuffer(void*, int, int);
    ~OpenGLVertexBuffer() override;

    void bind() override;
    void unbind() override;

    unsigned int getVerticesCount() override;

private:
    unsigned m_bufferId;
    unsigned m_verticesCount;
};
}