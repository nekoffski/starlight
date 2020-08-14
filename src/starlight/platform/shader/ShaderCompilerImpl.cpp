#include "ShaderCompilerImpl.h"

#include "starlight/core/log/Logger.h"
#include "starlight/platform/PlatformDetector.h"
#include "starlight/platform/shader/Shader.h"

#ifdef STARL_USE_OPENGL
#include "starlight/platform/shader/opengl/OpenGLShaderCompilerImpl.h"
#endif

namespace sl::platform::shader {

std::unique_ptr<ShaderCompilerImpl> ShaderCompilerImpl::create() {
#ifdef STARL_USE_OPENGL
    SL_INFO("instancing opengl shader compiler implementation");
    return std::make_unique<opengl::OpenGLShaderCompilerImpl>();
#endif
}
}
