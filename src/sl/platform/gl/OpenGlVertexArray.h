#pragma once

#include "sl/gfx/buffer/VertexArray.h"

namespace sl::platform::gl {

class OpenGlVertexArray : public sl::gfx::buffer::VertexArray {
public:
    OpenGlVertexArray();
    ~OpenGlVertexArray() override;

    void addVertexBuffer(std::shared_ptr<sl::gfx::buffer::VertexBuffer>) override;
    void addElementBuffer(std::shared_ptr<sl::gfx::buffer::ElementBuffer>) override;

    void bind() override;
    void unbind() override;
    unsigned int getVerticesCount() override;
    unsigned int getIndicesCount() override;

private:
    unsigned int m_bufferId;
    unsigned int m_verticesCount;
    unsigned int m_indicesCount;

    std::vector<std::shared_ptr<sl::gfx::buffer::VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<sl::gfx::buffer::ElementBuffer> m_elementBuffer;
};

struct OpenGlVertexArrayFactory : sl::gfx::buffer::VertexArray::Factory {
    std::shared_ptr<sl::gfx::buffer::VertexArray> create() {
        return std::make_shared<OpenGlVertexArray>();
    }
};
}
