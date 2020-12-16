#include "Shader.h"

#include "sl/core/Logger.h"
#include "sl/platform/PlatformDetector.h"

#ifdef STARL_USE_OPENGL
#include "sl/platform/shader/opengl/OpenGLShader.h"
#endif

namespace sl::platform::shader {

std::shared_ptr<Shader> Shader::create(std::string vertexPath, std::string fragmentPath, std::string geometryPath) {
#ifdef STARL_USE_OPENGL
    SL_DEBUG("instancing opengl shader");
    return std::make_shared<opengl::OpenGLShader>(vertexPath, fragmentPath, geometryPath);
#endif
}
}
