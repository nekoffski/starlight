#pragma once

namespace starl::platform::gpu {

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;
};
}
