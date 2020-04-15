#include <starlight/platform/gpu/VertexBuffer.h>

#include <starlight/platform/gpu/opengl/OpenGLVertexBuffer.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::gpu {
std::shared_ptr<VertexBuffer> VertexBuffer::create(misc::types::NotNullPtr<void> data, int size, int verticesCount) {
    LOG(DEBUG) << "instancing opengl vertex buffer";
    return std::make_shared<opengl::OpenGLVertexBuffer>(data, size, verticesCount);
}
}