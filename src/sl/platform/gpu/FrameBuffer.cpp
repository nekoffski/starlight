#include "FrameBuffer.h"

#include "opengl/OpenGLFrameBuffer.h"
#include "sl/core/log/Logger.h"

namespace sl::platform::gpu {
std::shared_ptr<FrameBuffer> FrameBuffer::create() {
    SL_DEBUG("instancing opengl frame buffer");
    return std::make_shared<opengl::OpenGLFrameBuffer>();
}
}
