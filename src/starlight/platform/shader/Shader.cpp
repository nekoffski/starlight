#include <starlight/platform/shader/Shader.h>

#include <starlight/platform/PlatformDetector.h>

#ifdef STARL_USE_OPENGL
#include <starlight/platform/shader/opengl/OpenGLShader.h>
#endif

namespace starl::platform::shader {

std::shared_ptr<Shader> Shader::create(std::string vertexPath, std::string fragmentPath, std::string geometryPath) {
#ifdef STARL_USE_OPENGL
    return std::make_shared<opengl::OpenGLShader>(vertexPath, fragmentPath, geometryPath);
#endif
}

Shader::Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath)
    : m_vertexPath(std::move(vertexPath))
    , m_fragmentPath(std::move(fragmentPath))
    , m_geometryPath(std::move(geometryPath)) {
}
}
