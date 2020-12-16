#include "RenderAPI.h"

#include "opengl/OpenGLRenderAPI.h"
#include "sl/core/Logger.h"

namespace sl::platform::gpu {

std::unique_ptr<RenderAPI> RenderAPI::create() {
    SL_INFO("instancing opengl render api");
    return std::make_unique<opengl::OpenGLRenderAPI>();
}
}