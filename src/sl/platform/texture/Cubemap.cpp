#include "Cubemap.h"

#include "opengl/OpenGLCubemap.h"
#include "sl/core/Logger.h"

namespace sl::platform::texture {

std::shared_ptr<Cubemap> Cubemap::create(const CubemapArgs cubemapArgs) {
    SL_DEBUG("instancing opengl cubemap");
    return std::make_shared<opengl::OpenGLCubemap>(std::move(cubemapArgs));
}
}