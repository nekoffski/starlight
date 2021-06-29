#include "OpenGlTexture.h"

#include <glad/glad.h>

#include "Utils.hpp"
#include "sl/core/ErrorCode.h"
#include "sl/core/Errors.hpp"

namespace sl::platform::gfx {

OpenGlTexture::OpenGlTexture(sl::gfx::Image& image)
    : m_textureId(0u) {

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    auto s = image.getSize();

    m_w = s.width;
    m_h = s.height;

    const auto& format = channelsToFormat.find(image.getChannels());

    if (format == channelsToFormat.end())
        throw core::TextureError { core::ErrorCode::UnknownTextureFormat };

    glTexImage2D(GL_TEXTURE_2D, 0, format->second, s.width, s.height, 0, format->second,
        GL_UNSIGNED_BYTE, image.getRaw());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGlTexture::OpenGlTexture(unsigned int w, unsigned int h)
    : m_w(w)
    , m_h(h) {

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        m_w, m_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGlTexture::~OpenGlTexture() {
    if (m_textureId)
        glDeleteTextures(1, &m_textureId);
}

void OpenGlTexture::bind(unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    m_lastBoundIndex = index;
}

void OpenGlTexture::unbind() {
    glActiveTexture(GL_TEXTURE0 + m_lastBoundIndex);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int OpenGlTexture::getWidth() const {
    return m_w;
}

unsigned int OpenGlTexture::getHeight() const {
    return m_h;
}

unsigned int OpenGlTexture::getBuffer() const {
    return m_textureId;
}
}
