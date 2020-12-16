#include "OpenGLVertexArray.h"

#include <glad/glad.h>

#include "sl/core/Logger.h"
#include "sl/platform/gpu/ElementBuffer.h"
#include "sl/platform/gpu/VertexBuffer.h"

namespace sl::platform::gpu::opengl {

OpenGLVertexArray::OpenGLVertexArray()
    : m_bufferId(0u)
    , m_verticesCount(0u)
    , m_indicesCount(0u) {
    glGenVertexArrays(1, &m_bufferId);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    if (m_bufferId)
        glDeleteBuffers(1, &m_bufferId);
}

void OpenGLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) {
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

void OpenGLVertexArray::addElementBuffer(std::shared_ptr<ElementBuffer> elementBuffer) {
    this->bind();
    elementBuffer->bind();

    m_indicesCount = elementBuffer->getIndicesCount();
    m_elementBuffer = elementBuffer;

    this->unbind();
    elementBuffer->unbind();
}

void OpenGLVertexArray::bind() {
    glBindVertexArray(m_bufferId);
    // LOG(DEBUG) << "Binding: " << m_bufferId;
}

void OpenGLVertexArray::unbind() {
    glBindVertexArray(0u);
}

unsigned int OpenGLVertexArray::getVerticesCount() {
    return m_verticesCount;
}

unsigned int OpenGLVertexArray::getIndicesCount() {
    return m_indicesCount;
}
}