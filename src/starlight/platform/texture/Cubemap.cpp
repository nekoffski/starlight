#include <starlight/platform/texture/Cubemap.h>

#include <starlight/platform/texture/opengl/OpenGLCubemap.h>

namespace starl::platform::texture {

std::shared_ptr<Cubemap> Cubemap::create(const CubemapArgs cubemapArgs) {
    return std::make_shared<opengl::OpenGLCubemap>(std::move(cubemapArgs));
}
}