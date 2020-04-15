#include <starlight/platform/gpu/VertexArray.h>

#include <starlight/platform/gpu/opengl/OpenGLVertexArray.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::gpu {

std::shared_ptr<VertexArray> VertexArray::create() {
    LOG(DEBUG) << "instancing opengl vertex array";
    return std::make_shared<opengl::OpenGLVertexArray>();
}
}
