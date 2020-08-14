#pragma once

#include <memory>
#include <vector>

namespace sl::platform::gpu {

class VertexBuffer;
class ElementBuffer;

class VertexArray {
public:
    static std::shared_ptr<VertexArray> create();

    virtual ~VertexArray() = default;

    virtual unsigned int getVerticesCount() = 0;
    virtual unsigned int getIndicesCount() = 0;
    virtual void addVertexBuffer(std::shared_ptr<VertexBuffer>) = 0;
    virtual void addElementBuffer(std::shared_ptr<ElementBuffer>) = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;
};
}