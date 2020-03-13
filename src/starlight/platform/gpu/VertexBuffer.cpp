#include <starlight/platform/gpu/VertexBuffer.h>

#include <starlight/platform/gpu/opengl/OpenGLVertexBuffer.h>

namespace starl::platform::gpu {
std::shared_ptr<VertexBuffer> VertexBuffer::create(misc::types::NotNullPtr<void> data, int size) {
    return std::make_shared<opengl::OpenGLVertexBuffer>(data, size);
}
}