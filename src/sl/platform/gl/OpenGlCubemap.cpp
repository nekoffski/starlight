#include "OpenGlCubemap.h"

#include <glad/glad.h>

#include <kc/core/Log.h>

#include "Utils.hpp"
#include "sl/core/Errors.hpp"
#include "sl/gfx/Image.h"

namespace sl::platform::gl {

OpenGlCubemap::OpenGlCubemap(unsigned int width, unsigned int height)
    : m_cubemapId(0u) {
    LOG_INFO("Creating cubemap without textures attached");

    glGenTextures(1, &m_cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapId);

    for (unsigned int i = 0u; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
            width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    this->width = width;
    this->height = height;
}

OpenGlCubemap::OpenGlCubemap(const sl::gfx::CubemapFaces& faces)
    : m_cubemapId(0u) {

    glGenTextures(1, &m_cubemapId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapId);

    LOG_INFO("Loading cubemap");

    const auto facesLen = faces.size();
    for (int i = 0; i < facesLen; ++i) {
        const auto& img = faces[i];

        auto channels = img->getChannelsCount();

        if (not channelsToFormat.contains(channels))
            throw core::TextureError {};

        const auto format = channelsToFormat.at(channels);
        const auto size = img->getSize();

        LOG_DEBUG("Face: {}, width: {}, height: {}", faces[i]->getPath(), size.width, size.height);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, size.width, size.height,
            0, format, GL_UNSIGNED_BYTE, img->getBuffer());

        // TODO: check if without 'if' below statement is faster
        if (i == 0) {
            this->width = size.width;
            this->height = size.height;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

OpenGlCubemap::~OpenGlCubemap() {
    if (m_cubemapId)
        glDeleteTextures(1, &m_cubemapId);
}

void OpenGlCubemap::bind(unsigned int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapId);

    m_lastBoundIndex = index;
}

void OpenGlCubemap::unbind() {
    glActiveTexture(GL_TEXTURE0 + m_lastBoundIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0u);
}
}
