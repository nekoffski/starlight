#include <starlight/platform/PlatformDetector.h>
#include <starlight/platform/shader/Shader.h>
#include <starlight/platform/shader/ShaderCompilerImpl.h>

#ifdef STARL_USE_OPENGL
#include <starlight/platform/shader/opengl/OpenGLShaderCompilerImpl.h>
#endif

#include <starlight/core/log/Logger.h>

namespace starl::platform::shader {

std::unique_ptr<ShaderCompilerImpl> ShaderCompilerImpl::create() {
#ifdef STARL_USE_OPENGL
    LOG(INFO) << "instancing opengl shader compiler implementation";
    return std::make_unique<opengl::OpenGLShaderCompilerImpl>();
#endif
}
}
