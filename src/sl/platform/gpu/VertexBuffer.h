#pragma once

#include <memory>

#include "BufferMemoryScheme.hpp"
#include "sl/misc/types/NotNullPtr.hpp"

// TODO: IFDEF
#include "sl/platform/gpu/opengl/OpenGLDefines.h"

namespace sl::platform::gpu {

class VertexBuffer {
public:
    static std::shared_ptr<VertexBuffer> create(misc::types::NotNullPtr<void>, int, int);

    virtual ~VertexBuffer() = default;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual unsigned int getVerticesCount() = 0;

    const BufferMemoryScheme& getMemoryScheme() {
        return m_bufferMemoryScheme;
    }

    void addMemoryOffsetScheme(uint32_t elementsCount, uint32_t type, uint32_t typeSize, bool normalized = false) {
        m_bufferMemoryScheme.addScheme(elementsCount, type, typeSize, normalized);
    }

protected:
    BufferMemoryScheme m_bufferMemoryScheme;
};
}
