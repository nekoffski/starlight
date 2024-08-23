#pragma once

#include "Vulkan.h"
#include "fwd.h"

#include "VKLogicalDevice.h"
#include "VKContext.h"

namespace sl::vk {

class VKFence {
public:
    enum class State : unsigned char { signaled, notSignaled };

    explicit VKFence(VKContext& context, VKLogicalDevice& device, State state);
    ~VKFence();

    bool wait(Nanoseconds timeout);
    void reset();

    VkFence getHandle();

private:
    VkFence m_handle;

    VKContext& m_context;
    VKLogicalDevice& m_device;

    State m_state;
};

}  // namespace sl::vk
