#include "OpenGlFrameBuffer.h"

#include "sl/gfx/Texture.h"

#include <glad/glad.h>

namespace sl::platform::gpu {

OpenGlFrameBuffer::OpenGlFrameBuffer()
    : m_bufferId(0u) {
    glGenFramebuffers(1, &m_bufferId);
}

OpenGlFrameBuffer::~OpenGlFrameBuffer() {
    if (m_bufferId)
        glDeleteFramebuffers(1, &m_bufferId);
}

void OpenGlFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);
}

void OpenGlFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}

void OpenGlFrameBuffer::bindTexture(std::shared_ptr<sl::gfx::Texture> texture) {
    // TODO: make it configurable
    //    texture->bind();
    //	bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getBuffer(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    //  unbind();
    //texture->unbind();
}
}
