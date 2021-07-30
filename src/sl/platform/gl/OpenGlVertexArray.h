#pragma once

#include "sl/gfx/VertexArray.h"

namespace sl::platform::gl {

class OpenGlVertexArray : public sl::gfx::VertexArray {
public:
    OpenGlVertexArray();
    ~OpenGlVertexArray() override;

    void addVertexBuffer(std::shared_ptr<sl::gfx::VertexBuffer>) override;
    void addElementBuffer(std::shared_ptr<sl::gfx::ElementBuffer>) override;

    void bind() override;
    void unbind() override;
    unsigned int getVerticesCount() override;
    unsigned int getIndicesCount() override;

private:
    unsigned int m_bufferId;
    unsigned int m_verticesCount;
    unsigned int m_indicesCount;

    std::vector<std::shared_ptr<sl::gfx::VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<sl::gfx::ElementBuffer> m_elementBuffer;
};

struct OpenGlVertexArrayFactory : gfx::VertexArray::Factory {
    std::shared_ptr<gfx::VertexArray> create() {
        return std::make_shared<OpenGlVertexArray>();
    }
};
}
