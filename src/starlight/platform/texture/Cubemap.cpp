#include <starlight/platform/texture/Cubemap.h>

#include <starlight/platform/texture/opengl/OpenGLCubemap.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::texture {

std::shared_ptr<Cubemap> Cubemap::create(const CubemapArgs cubemapArgs) {
    LOG(DEBUG) << "instancing opengl cubemap";
    return std::make_shared<opengl::OpenGLCubemap>(std::move(cubemapArgs));
}
}