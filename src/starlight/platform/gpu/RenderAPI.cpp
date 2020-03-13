#include <starlight/platform/gpu/RenderAPI.h>

#include <starlight/platform/gpu/opengl/OpenGLRenderAPI.h>

namespace starl::platform::gpu {

std::unique_ptr<RenderAPI> RenderAPI::create() {
    return std::make_unique<opengl::OpenGLRenderAPI>();
}
}