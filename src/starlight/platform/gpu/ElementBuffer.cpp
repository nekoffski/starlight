#include "ElementBuffer.h"

#include "opengl/OpenGLElementBuffer.h"
#include "starlight/core/log/Logger.h"

namespace sl::platform::gpu {
std::shared_ptr<ElementBuffer> ElementBuffer::create(misc::types::NotNullPtr<void> data, int size, int indicesCount) {
    SL_DEBUG("instancing opengl element buffer");
    return std::make_shared<opengl::OpenGLElementBuffer>(data, size, indicesCount);
}
}