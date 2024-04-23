#pragma once

#include "Vulkan.h"
#include "fwd.h"

#include "VKPhysicalDevice.h"
#include "VKContext.h"
#include "VKBackendAccessor.h"

namespace sl::vk {

class VKFence {
public:
    enum class State : unsigned char { signaled, notSignaled };

    explicit VKFence(VKBackendAccessor& backendAccessor, State state);
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
