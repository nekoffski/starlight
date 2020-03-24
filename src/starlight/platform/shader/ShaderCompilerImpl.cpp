#include <starlight/platform/PlatformDetector.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/shader/ShaderCompilerImpl.h>

#ifdef STARL_USE_OPENGL
#include <starlight/platform/shader/opengl/OpenGLShaderCompilerImpl.h>
#endif

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("ShaderCompilerImpl");

namespace starl::platform::shader {

std::unique_ptr<ShaderCompilerImpl> ShaderCompilerImpl::create() {
#ifdef STARL_USE_OPENGL
    logger->info("instancing opengl shader compiler implementation");
    return std::make_unique<opengl::OpenGLShaderCompilerImpl>();
#endif
}
}
