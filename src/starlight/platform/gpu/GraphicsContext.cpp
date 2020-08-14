#include "GraphicsContext.h"

#include "opengl/OpenGLGraphicsContext.h"
#include "starlight/core/log/Logger.h"
#include "starlight/platform/PlatformDetector.h"

namespace sl::platform::gpu {

std::unique_ptr<GraphicsContext> GraphicsContext::create(misc::types::NotNullPtr<void> windowHandle) {
#ifdef STARL_USE_OPENGL
    SL_INFO("instancing opengl graphics context");
    return std::make_unique<opengl::OpenGLGraphicsContext>(windowHandle);
#endif
}
}
