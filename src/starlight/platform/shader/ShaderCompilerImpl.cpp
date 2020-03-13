#include <starlight/platform/PlatformDetector.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/shader/ShaderCompilerImpl.h>

#ifdef STARL_USE_OPENGL
#include <starlight/platform/shader/opengl/OpenGLShaderCompilerImpl.h>
#endif

namespace starl::platform::shader {

std::unique_ptr<ShaderCompilerImpl> ShaderCompilerImpl::create() {
#ifdef STARL_USE_OPENGL
    return std::make_unique<opengl::OpenGLShaderCompilerImpl>();
#endif
}
}
