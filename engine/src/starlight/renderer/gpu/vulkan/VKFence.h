#pragma once

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class VKFence {
public:
    enum class State : unsigned char { signaled, notSignaled };

    explicit VKFence(const VKContext* context, const VKDevice* device, State state);
    ~VKFence();

    bool wait(Nanoseconds timeout);
    void reset();

    VkFence getHandle();

private:
    VkFence m_handle;

    const VKContext* m_context;
    const VKDevice* m_device;

    State m_state;
};

}  // namespace sl::vk
