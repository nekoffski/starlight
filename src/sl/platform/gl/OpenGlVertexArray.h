#pragma once

#include "sl/gpu/VertexArray.h"

namespace sl::platform::gl {

class OpenGlVertexArray : public sl::gpu::VertexArray {
   public:
    OpenGlVertexArray();
    ~OpenGlVertexArray() override;

    void addVertexBuffer(std::shared_ptr<sl::gpu::VertexBuffer>) override;
    void addElementBuffer(std::shared_ptr<sl::gpu::ElementBuffer>) override;

    void bind() override;
    void unbind() override;
    unsigned int getVerticesCount() override;
    unsigned int getIndicesCount() override;

   private:
    unsigned int m_bufferId;
    unsigned int m_verticesCount;
    unsigned int m_indicesCount;

    std::vector<std::shared_ptr<sl::gpu::VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<sl::gpu::ElementBuffer> m_elementBuffer;
};

struct OpenGlVertexArrayFactory : gpu::VertexArray::Factory {
    std::shared_ptr<gpu::VertexArray> create() { return std::make_shared<OpenGlVertexArray>(); }
};
}  // namespace sl::platform::gl
