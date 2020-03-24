#include <starlight/platform/texture/opengl/OpenGLCubemap.h>

#include <starlight/platform/Error.h>
#include <starlight/platform/img/Image.h>
#include <starlight/platform/texture/opengl/Utils.hpp>

#include <glad/glad.h>

namespace starl::platform::texture::opengl {

OpenGLCubemap::OpenGLCubemap(const CubemapArgs faces)
    : m_cubemapId(0u) {
    glGenTextures(1, &m_cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapId);

    const auto facesLen = faces.size();
    for (int i = 0; i < facesLen; ++i) {
        const auto img = img::Image::create(faces[i]);
        const auto format = channelsToFormat.find(img->getChannels());
        if (format == channelsToFormat.end()) {
            throw PlatformException(ErrorCode::UNKNOWN_TEXTURE_FORMAT);
        }
        const auto& size = img->getSize();
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format->second, size.width, size.height,
            0, format->second, GL_UNSIGNED_BYTE, img->getRaw());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

OpenGLCubemap::~OpenGLCubemap() {
    if (m_cubemapId) {
        glDeleteTextures(1, &m_cubemapId);
    }
}

void OpenGLCubemap::bind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapId);
}

void OpenGLCubemap::unbind() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0u);
}
}