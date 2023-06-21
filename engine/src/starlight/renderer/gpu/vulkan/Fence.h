#pragma once

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class Fence {
   public:
    enum class State : unsigned char { signaled, notSignaled };

    explicit Fence(const Context* context, const Device* device, State state);
    ~Fence();

    bool wait(Nanoseconds timeout);
    void reset();

    VkFence getHandle();

   private:
    VkFence m_handle;

    const Context* m_context;
    const Device* m_device;

    State m_state;
};

}  // namespace sl::vk
