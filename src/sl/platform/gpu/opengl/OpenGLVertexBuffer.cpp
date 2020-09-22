#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace sl::platform::gpu::opengl {

OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, int size, int verticesCount)
    : m_bufferId(0u)
    , m_verticesCount(verticesCount) {
    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    if (m_bufferId)
        glDeleteBuffers(1, &m_bufferId);
}

void OpenGLVertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
}

void OpenGLVertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}

unsigned int OpenGLVertexBuffer::getVerticesCount() {
    return m_verticesCount;
}
}
