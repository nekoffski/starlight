#pragma once

#include "Vulkan.h"

namespace nova::platform::vulkan {

struct CommandBuffer {
    enum class State : unsigned char {
        ready = 0,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    vk::raii::CommandBuffer handle;
    State state;
};

}  // namespace nova::platform::vulkan
