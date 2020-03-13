#include <starlight/platform/gpu/GraphicsContext.h>

#include <starlight/platform/PlatformDetector.h>

#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

namespace starl::platform::gpu {

std::unique_ptr<GraphicsContext> GraphicsContext::create(misc::types::NotNullPtr<void> windowHandle) {
#ifdef STARL_USE_OPENGL
    return std::make_unique<opengl::OpenGLGraphicsContext>(windowHandle);
#endif
}
}
