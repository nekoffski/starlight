#include "OpenGLFrameBuffer.h"

#include "sl/platform/texture/Texture.h"

#include <glad/glad.h>

namespace sl::platform::gpu::opengl {

OpenGLFrameBuffer::OpenGLFrameBuffer()
    : m_bufferId(0u) {
    glGenFramebuffers(1, &m_bufferId);
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
    if (m_bufferId)
        glDeleteFramebuffers(1, &m_bufferId);
}

void OpenGLFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);
}

void OpenGLFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}

void OpenGLFrameBuffer::bindTexture(std::shared_ptr<texture::Texture> texture) {
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
