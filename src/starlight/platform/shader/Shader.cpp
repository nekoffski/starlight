#include <starlight/platform/shader/Shader.h>

#include <starlight/platform/PlatformDetector.h>

#ifdef STARL_USE_OPENGL
#include <starlight/platform/shader/opengl/OpenGLShader.h>
#endif

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("Shader");

namespace starl::platform::shader {

std::shared_ptr<Shader> Shader::create(std::string vertexPath, std::string fragmentPath, std::string geometryPath) {
#ifdef STARL_USE_OPENGL
    logger->trace("instancing opengl shader");
    return std::make_shared<opengl::OpenGLShader>(vertexPath, fragmentPath, geometryPath);
#endif
}
}
