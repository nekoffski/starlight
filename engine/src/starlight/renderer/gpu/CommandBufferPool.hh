#pragma once

#include "CommandBuffer.hh"

namespace sl {

class CommandBufferPool : public NonCopyable, public NonMovable {
public:
    virtual CommandBuffer* getFrameCommandBuffer() = 0;
};

}  // namespace sl
