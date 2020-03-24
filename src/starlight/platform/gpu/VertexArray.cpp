#include <starlight/platform/gpu/VertexArray.h>

#include <starlight/platform/gpu/opengl/OpenGLVertexArray.h>

#include <starlight/core/log/Logger.h>

static auto logger = starl::core::log::createLogger("VertexArrayr");

namespace starl::platform::gpu {

std::shared_ptr<VertexArray> VertexArray::create() {
    logger->trace("instancing opengl vertex array");
    return std::make_shared<opengl::OpenGLVertexArray>();
}
}
