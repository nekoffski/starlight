#pragma once

#include <starlight/platform/gpu/VertexArray.h>

namespace starl::platform::gpu::opengl {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void addVertexBuffer(std::shared_ptr<VertexBuffer>) override;
    void addElementBuffer(std::shared_ptr<ElementBuffer>) override;

    void bind() override;
    void unbind() override;
    unsigned int getVerticesCount() override;
    unsigned int getIndicesCount() override;

private:
    unsigned int m_bufferId;
    unsigned int m_verticesCount;
    unsigned int m_indicesCount;

    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<ElementBuffer> m_elementBuffer;
};
}