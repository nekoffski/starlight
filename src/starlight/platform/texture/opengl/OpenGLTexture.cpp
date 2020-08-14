#include "OpenGLTexture.h"

#include <glad/glad.h>

#include "starlight/platform/Error.h"
#include "starlight/platform/texture/opengl/Utils.hpp"

namespace sl::platform::texture::opengl {

OpenGLTexture::OpenGLTexture(const std::string& path)
    : m_textureId(0u) {
    m_textureImage = img::Image::create(path);

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    auto s = m_textureImage->getSize();

    const auto& format = channelsToFormat.find(m_textureImage->getChannels());

    if (format == channelsToFormat.end()) {
        throw PlatformException(ErrorCode::UNKNOWN_TEXTURE_FORMAT);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format->second, s.width, s.height, 0, format->second,
        GL_UNSIGNED_BYTE, m_textureImage->getRaw());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTexture::~OpenGLTexture() {
    if (m_textureId)
        glDeleteTextures(1, &m_textureId);
}

void OpenGLTexture::bind(unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void OpenGLTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
}
