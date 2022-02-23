#pragma once

#include <kc/core/Singleton.hpp>

#include "ElementBuffer.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace sl::gfx {

class BufferManager : public kc::core::Singleton<BufferManager> {
   public:
    std::shared_ptr<FrameBuffer> createFrameBuffer() { return m_frameBufferFactory->create(); }

    std::unique_ptr<RenderBuffer> createRenderBuffer(int format, unsigned int width,
                                                     unsigned int height) {
        return m_renderBufferFactory->create(format, width, height);
    }

    std::shared_ptr<VertexArray> createVertexArray() { return m_vertexArrayFactory->create(); }

    std::shared_ptr<VertexBuffer> createVertexBuffer(void* data, int size, int verticesCount) {
        return m_vertexBufferFactory->create(data, size, verticesCount);
    }

    std::shared_ptr<ElementBuffer> createElementBuffer(void* data, int size,
                                                       unsigned int indicesCount) {
        return m_elementBufferFactory->create(data, size, indicesCount);
    }

    void setElementBufferFactory(ElementBuffer::Factory* elementBufferFactory) {
        m_elementBufferFactory = elementBufferFactory;
    }

    void setVertexBufferFactory(VertexBuffer::Factory* vertexBufferFactory) {
        m_vertexBufferFactory = vertexBufferFactory;
    }

    void setFrameBufferFactory(FrameBuffer::Factory* frameBufferFactory) {
        m_frameBufferFactory = frameBufferFactory;
    }
    void setRenderBufferFactory(RenderBuffer::Factory* renderBufferFactory) {
        m_renderBufferFactory = renderBufferFactory;
    }

    void setVertexArrayFactory(VertexArray::Factory* vertexArrayFactory) {
        m_vertexArrayFactory = vertexArrayFactory;
    }

   private:
    ElementBuffer::Factory* m_elementBufferFactory;
    VertexBuffer::Factory* m_vertexBufferFactory;
    FrameBuffer::Factory* m_frameBufferFactory;
    RenderBuffer::Factory* m_renderBufferFactory;
    VertexArray::Factory* m_vertexArrayFactory;
};

}  // namespace sl::gfx