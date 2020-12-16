#include "VertexArray.h"

#include "opengl/OpenGLVertexArray.h"
#include "sl/core/Logger.h"

namespace sl::platform::gpu {

std::shared_ptr<VertexArray> VertexArray::create() {
    SL_DEBUG("instancing opengl vertex array");
    return std::make_shared<opengl::OpenGLVertexArray>();
}
}
