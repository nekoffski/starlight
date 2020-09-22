#include "ShaderCompilerImpl.h"

#include "sl/core/log/Logger.h"
#include "sl/platform/PlatformDetector.h"
#include "sl/platform/shader/Shader.h"

#ifdef STARL_USE_OPENGL
#include "sl/platform/shader/opengl/OpenGLShaderCompilerImpl.h"
#endif

namespace sl::platform::shader {

std::unique_ptr<ShaderCompilerImpl> ShaderCompilerImpl::create() {
#ifdef STARL_USE_OPENGL
    SL_INFO("instancing opengl shader compiler implementation");
    return std::make_unique<opengl::OpenGLShaderCompilerImpl>();
#endif
}
}
