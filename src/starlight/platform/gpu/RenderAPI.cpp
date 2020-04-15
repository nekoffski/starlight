#include <starlight/platform/gpu/RenderAPI.h>

#include <starlight/platform/gpu/opengl/OpenGLRenderAPI.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::gpu {

std::unique_ptr<RenderAPI> RenderAPI::create() {
    LOG(INFO) << "instancing opengl render api";
    return std::make_unique<opengl::OpenGLRenderAPI>();
}
}