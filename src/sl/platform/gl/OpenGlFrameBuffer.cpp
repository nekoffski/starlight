#include "OpenGlFrameBuffer.h"

#include "sl/gfx/Texture.h"

#include <glad/glad.h>

namespace sl::platform::gl {

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

void OpenGlFrameBuffer::bindRenderBuffer(sl::gfx::RenderBuffer& buffer) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer.getBufferId());
}

void OpenGlFrameBuffer::bindTexture(sl::gfx::Texture& texture, unsigned int attachment) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.getBuffer(), 0);
}

void OpenGlFrameBuffer::bindTexture(sl::gfx::Texture& texture) {
    // TODO: make it configurable
    //    texture->bind();
    // bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.getBuffer(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    // unbind();
    //texture->unbind();
}

void OpenGlFrameBuffer::bindCubemap(sl::gfx::Cubemap& cubemap) {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubemap.getBufferId(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void OpenGlFrameBuffer::specifyColorBuffers(const std::vector<unsigned int>& buffers) {
    glDrawBuffers(buffers.size(), buffers.data());
}

}
