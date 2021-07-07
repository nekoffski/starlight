#include "OpenGlRenderBuffer.h"

#include <glad/glad.h>

namespace sl::platform::gfx {

OpenGlRenderBuffer::OpenGlRenderBuffer(int format, unsigned int width, unsigned int height) {
    glGenRenderbuffers(1, &m_buffferId);

    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    unbind();
}

OpenGlRenderBuffer::~OpenGlRenderBuffer() {
    glDeleteRenderbuffers(1, &m_buffferId);
}

unsigned int OpenGlRenderBuffer::getBufferId() const {
    return m_buffferId;
}

void OpenGlRenderBuffer::bind() {
    glBindRenderbuffer(GL_RENDERBUFFER, m_buffferId);
}

void OpenGlRenderBuffer::unbind() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0u);
}
}