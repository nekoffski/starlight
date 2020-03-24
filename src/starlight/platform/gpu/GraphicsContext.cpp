#include <starlight/platform/gpu/GraphicsContext.h>

#include <starlight/platform/PlatformDetector.h>

#include <starlight/platform/gpu/opengl/OpenGLGraphicsContext.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("GraphicsContext");

namespace starl::platform::gpu {

std::unique_ptr<GraphicsContext> GraphicsContext::create(misc::types::NotNullPtr<void> windowHandle) {
#ifdef STARL_USE_OPENGL
    logger->info("instancing opengl graphics context");
    return std::make_unique<opengl::OpenGLGraphicsContext>(windowHandle);
#endif
}
}
