#include <starlight/platform/gpu/opengl/OpenGLVertexArray.h>
#include <starlight/platform/gpu/VertexBuffer.h>

#include <glad/glad.h>

#include <iostream>

namespace starl::platform::gpu::opengl {

OpenGLVertexArray::OpenGLVertexArray()
    : m_bufferId(0u) {
    glGenVertexArrays(1, &m_bufferId);
}

OpenGLVertexArray::~OpenGLVertexArray() {
    if (m_bufferId) {
        glDeleteBuffers(1, &m_bufferId);
    }
}

void OpenGLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) {
    this->bind();
    vertexBuffer->bind();

    const auto& memoryScheme = vertexBuffer->getMemoryScheme();

    for (const auto& offset : memoryScheme) {
        glVertexAttribPointer(offset.index, offset.elementsCount, offset.type, offset.normalized, memoryScheme.calculateStride(), offset.begin);
        glEnableVertexAttribArray(offset.index);
    }

    vertexBuffer->unbind();
    this->unbind();
}

void OpenGLVertexArray::bind() {
    glBindVertexArray(m_bufferId);
}

void OpenGLVertexArray::unbind() {
    glBindVertexArray(0u);
}
}