#include "OpenGLElementBuffer.h"

#include <glad/glad.h>

#include "sl/core/log/Logger.h"

namespace sl::platform::gpu::opengl {

OpenGLElementBuffer::OpenGLElementBuffer(void* data, int size, unsigned int indicesCount)
    : m_indicesCount(indicesCount)
    , m_bufferId(0u) {
    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

OpenGLElementBuffer::~OpenGLElementBuffer() {
    if (m_bufferId)
        glDeleteBuffers(1, &m_bufferId);
}

void OpenGLElementBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
    // LOG(DEBUG) << "Binding EBO: " << m_bufferId;
}

void OpenGLElementBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}
}