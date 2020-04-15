#include <starlight/platform/texture/Texture.h>

#include <starlight/platform/texture/opengl/OpenGLTexture.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::texture {

std::shared_ptr<Texture> Texture::create(const std::string& path) {
    LOG(DEBUG) << "instancing opengl texture";
    return std::make_shared<opengl::OpenGLTexture>(path);
}
}