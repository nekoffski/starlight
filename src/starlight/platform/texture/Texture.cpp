#include <starlight/platform/texture/Texture.h>

#include <starlight/platform/texture/opengl/OpenGLTexture.h>

namespace starl::platform::texture {

std::shared_ptr<Texture> Texture::create(const std::string& path) {
    return std::make_shared<opengl::OpenGLTexture>(path);
}
}