#pragma once

#include "Vulkan.h"
#include "fwd.h"

namespace nova::platform::vulkan {

class Fence {
   public:
    enum class State : unsigned char { signaled, notSignaled };

    explicit Fence(const Context* context, const Device* device, State state);
    ~Fence();

    bool wait(uint64_t timeout);
    void reset();

   private:
    VkFence m_handle;

    const Context* m_context;
    const Device* m_device;

    State m_state;
};

}  // namespace nova::platform::vulkan
