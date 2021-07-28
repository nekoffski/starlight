#pragma once

#include <memory>

#include "BufferMemoryScheme.hpp"
#include "sl/core/NotNullPtr.hpp"

// TODO: IFDEF
#include "sl/platform/gl/OpenGlDefines.h"

namespace sl::gfx {

class VertexBuffer {
public:
    struct Factory {
        virtual std::shared_ptr<VertexBuffer> create(core::NotNullPtr<void>, int, int) = 0;
        virtual ~Factory() = default;
    };

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
