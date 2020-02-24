#include <starlight/platform/gpu/GraphicsContext.h>

#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

namespace starl::platform::gpu {

std::unique_ptr<GraphicsContext> GraphicsContext::create(void* windowHandle) {
    return std::make_unique<opengl::OpenGLGraphicsContext>(windowHandle);
}
}