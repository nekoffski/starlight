#pragma once

#include "CommandBuffer.h"

namespace sl {

struct CommandBufferPool {
    virtual CommandBuffer* getFrameCommandBuffer() = 0;
};

}  // namespace sl
