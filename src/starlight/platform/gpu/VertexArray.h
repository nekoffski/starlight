#pragma once

#include <vector>
#include <memory>

namespace starl::platform::gpu {

class VertexBuffer;

class VertexArray {
public:
    static std::shared_ptr<VertexArray> create();

    virtual ~VertexArray() = default;

    virtual void addVertexBuffer(std::shared_ptr<VertexBuffer>&) = 0;
    virtual void bind() = 0;
    virtual void unbind() = 0;
};
}