#include "OpenGlVertexArray.h"

#include <glad/glad.h>

#include "sl/core/Logger.h"
#include "sl/gfx/buffer/ElementBuffer.h"
#include "sl/gfx/buffer/VertexBuffer.h"

namespace sl::platform::gpu {

OpenGlVertexArray::OpenGlVertexArray()
    : m_bufferId(0u)
    , m_verticesCount(0u)
    , m_indicesCount(0u) {
    glGenVertexArrays(1, &m_bufferId);
}

OpenGlVertexArray::~OpenGlVertexArray() {
    if (m_bufferId)
        glDeleteBuffers(1, &m_bufferId);
}

void OpenGlVertexArray::addVertexBuffer(std::shared_ptr<gfx::buffer::VertexBuffer> vertexBuffer) {
    this->bind();
    vertexBuffer->bind();

    const auto& memoryScheme = vertexBuffer->getMemoryScheme();

    for (const auto& offset : memoryScheme) {
        glVertexAttribPointer(offset.index, offset.elementsCount, offset.type, offset.normalized, memoryScheme.calculateStride(), offset.begin);
        glEnableVertexAttribArray(offset.index);
    }

    vertexBuffer->unbind();
    this->unbind();

    m_verticesCount += vertexBuffer->getVerticesCount();
    m_vertexBuffers.push_back(vertexBuffer);
}

void OpenGlVertexArray::addElementBuffer(std::shared_ptr<gfx::buffer::ElementBuffer> elementBuffer) {
    this->bind();
    elementBuffer->bind();

    m_indicesCount = elementBuffer->getIndicesCount();
    m_elementBuffer = elementBuffer;

    this->unbind();
    elementBuffer->unbind();
}

void OpenGlVertexArray::bind() {
    glBindVertexArray(m_bufferId);
    // LOG(DEBUG) << "Binding: " << m_bufferId;
}

void OpenGlVertexArray::unbind() {
    glBindVertexArray(0u);
}

unsigned int OpenGlVertexArray::getVerticesCount() {
    return m_verticesCount;
}

unsigned int OpenGlVertexArray::getIndicesCount() {
    return m_indicesCount;
}
}
