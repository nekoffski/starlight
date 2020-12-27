#include "OpenGlTexture.h"

#include <glad/glad.h>

#include "Utils.hpp"
#include "sl/core/error/ErrorCode.h"
#include "sl/core/error/Errors.hpp"

namespace sl::platform::texture {

OpenGlTexture::OpenGlTexture(const std::string& path)
    : m_textureId(0u)
    , m_textureImage(graphics::Image::factory->create(path)) {

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    auto s = m_textureImage->getSize();

    m_w = s.width;
    m_h = s.height;

    const auto& format = channelsToFormat.find(m_textureImage->getChannels());

    if (format == channelsToFormat.end())
        throw core::error::TextureError{ core::error::ErrorCode::UnknownTextureFormat };

    glTexImage2D(GL_TEXTURE_2D, 0, format->second, s.width, s.height, 0, format->second,
        GL_UNSIGNED_BYTE, m_textureImage->getRaw());
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGlTexture::~OpenGlTexture() {
    if (m_textureId)
        glDeleteTextures(1, &m_textureId);
}

void OpenGlTexture::bind(unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void OpenGlTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
}
