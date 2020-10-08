#include "Texture.h"

#include "opengl/OpenGLTexture.h"
#include "sl/core/log/Logger.h"

namespace sl::platform::texture {

std::shared_ptr<Texture> Texture::create(const std::string& path) {
    SL_DEBUG("instancing opengl texture");
    return std::make_shared<opengl::OpenGLTexture>(path);
}

std::shared_ptr<Texture> Texture::create(unsigned int w, unsigned int h) {
    SL_DEBUG("instancing opengl texture");
    return std::make_shared<opengl::OpenGLTexture>(w, h);
}
}
