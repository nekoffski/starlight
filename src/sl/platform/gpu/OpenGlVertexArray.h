#pragma once

#include "sl/graphics/buffer/VertexArray.h"
#include "sl/platform/gpu/fwd.h"

namespace sl::platform::gpu {

class OpenGlVertexArray : public graphics::buffer::VertexArray {
public:
    OpenGlVertexArray();
    ~OpenGlVertexArray() override;

    void addVertexBuffer(std::shared_ptr<graphics::buffer::VertexBuffer>) override;
    void addElementBuffer(std::shared_ptr<graphics::buffer::ElementBuffer>) override;

    void bind() override;
    void unbind() override;
    unsigned int getVerticesCount() override;
    unsigned int getIndicesCount() override;

private:
    unsigned int m_bufferId;
    unsigned int m_verticesCount;
    unsigned int m_indicesCount;

    std::vector<std::shared_ptr<graphics::buffer::VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<graphics::buffer::ElementBuffer> m_elementBuffer;
};

struct OpenGlVertexArrayFactory : graphics::buffer::VertexArray::Factory {
    std::shared_ptr<graphics::buffer::VertexArray> create() {
        return std::make_shared<OpenGlVertexArray>();
    }
};
}
