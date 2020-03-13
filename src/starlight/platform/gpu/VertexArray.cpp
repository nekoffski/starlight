#include <starlight/platform/gpu/VertexArray.h>

#include <starlight/platform/gpu/opengl/OpenGLVertexArray.h>

namespace starl::platform::gpu {

std::shared_ptr<VertexArray> VertexArray::create() {
    return std::make_shared<opengl::OpenGLVertexArray>();
}
}
