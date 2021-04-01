#include "OpenGlElementBuffer.h"

#include <glad/glad.h>

#include "sl/core/Logger.h"

namespace sl::platform::gfx {

OpenGlElementBuffer::OpenGlElementBuffer(void* data, int size, unsigned int indicesCount)
    : m_indicesCount(indicesCount)
    , m_bufferId(0u) {
    glGenBuffers(1, &m_bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}

OpenGlElementBuffer::~OpenGlElementBuffer() {
    if (m_bufferId)
        glDeleteBuffers(1, &m_bufferId);
}

void OpenGlElementBuffer::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
}

void OpenGlElementBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
}
}
