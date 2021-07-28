#pragma once

#include <memory>

#include "sl/core/NotNullPtr.hpp"

namespace sl::gfx {

class ElementBuffer {
public:
    struct Factory {
        virtual std::shared_ptr<ElementBuffer> create(core::NotNullPtr<void>, int, unsigned int) = 0;
        virtual ~Factory() = default;
    };

    virtual ~ElementBuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual unsigned int getIndicesCount() = 0;
};
}
