#include <starlight/platform/gpu/RenderAPI.h>

#include <starlight/platform/gpu/opengl/OpenGLRenderAPI.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("RenderAPI");

namespace starl::platform::gpu {

std::unique_ptr<RenderAPI> RenderAPI::create() {
    logger->info("instancing opengl render api");
    return std::make_unique<opengl::OpenGLRenderAPI>();
}
}