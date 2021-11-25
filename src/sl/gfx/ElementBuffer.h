#pragma once

#include <memory>

namespace sl::gfx {

class ElementBuffer {
public:
    struct Factory {
        virtual std::shared_ptr<ElementBuffer> create(void* data, int, unsigned int) = 0;
        virtual ~Factory() = default;
    };

    virtual ~ElementBuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual unsigned int getIndicesCount() = 0;
};
}
