#pragma once

#include "CommandBuffer.hh"

namespace sl {

struct CommandBufferPool {
    virtual CommandBuffer* getFrameCommandBuffer() = 0;
};

}  // namespace sl
