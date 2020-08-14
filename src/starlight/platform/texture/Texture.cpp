#include "Texture.h"

#include "opengl/OpenGLTexture.h"
#include "starlight/core/log/Logger.h"

namespace sl::platform::texture {

std::shared_ptr<Texture> Texture::create(const std::string& path) {
    SL_DEBUG("instancing opengl texture");
    return std::make_shared<opengl::OpenGLTexture>(path);
}
}