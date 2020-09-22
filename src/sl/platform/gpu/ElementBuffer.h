#pragma once

#include <memory>

#include "sl/misc/types/NotNullPtr.hpp"

namespace sl::platform::gpu {

class ElementBuffer {
public:
    static std::shared_ptr<ElementBuffer> create(misc::types::NotNullPtr<void>, int, int);

    virtual ~ElementBuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual unsigned int getIndicesCount() = 0;
};
}
