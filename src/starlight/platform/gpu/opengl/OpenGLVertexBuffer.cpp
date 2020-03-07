#include <starlight/platform/gpu/opengl/OpenGLVertexBuffer.h>

#include <glad/glad.h>

namespace starl::platform::gpu::opengl {

OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, int size) {
    glCreateBuffers(1, &m_bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_bufferId);
}

void OpenGLVertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
}

void OpenGLVertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0u);
}
}
