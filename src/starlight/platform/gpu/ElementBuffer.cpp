#include <starlight/platform/gpu/ElementBuffer.h>

#include <starlight/platform/gpu/opengl/OpenGLElementBuffer.h>

#include <starlight/core/log/Logger.h>

namespace starl::platform::gpu {
std::shared_ptr<ElementBuffer> ElementBuffer::create(misc::types::NotNullPtr<void> data, int size, int indicesCount) {
    LOG(DEBUG) << "instancing opengl element buffer";
    return std::make_shared<opengl::OpenGLElementBuffer>(data, size, indicesCount);
}
}