#include "VertexBuffer.h"

#include "opengl/OpenGLVertexBuffer.h"
#include "sl/core/log/Logger.h"

namespace sl::platform::gpu {
std::shared_ptr<VertexBuffer> VertexBuffer::create(misc::types::NotNullPtr<void> data, int size, int verticesCount) {
    SL_DEBUG("instancing opengl vertex buffer");
    return std::make_shared<opengl::OpenGLVertexBuffer>(data, size, verticesCount);
}
}