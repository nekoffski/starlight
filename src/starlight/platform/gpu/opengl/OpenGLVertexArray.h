#pragma once

#include <starlight/platform/gpu/VertexArray.h>

namespace starl::platform::gpu::opengl {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void addVertexBuffer(std::shared_ptr<VertexBuffer>&) override;
    void bind() override;
    void unbind() override;

private:
    unsigned m_bufferId;
};
}