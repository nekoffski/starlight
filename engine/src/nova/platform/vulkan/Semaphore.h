#pragma once

#include "Vulkan.h"

#include "fwd.h"

namespace nova::platform::vulkan {

class Semaphore {
   public:
    explicit Semaphore(const Context* context, const Device* device);
    ~Semaphore();

    VkSemaphore getHandle() const;
    VkSemaphore* getHandlePointer();

   private:
    const Context* m_context;
    const Device* m_device;

    VkSemaphore m_handle;
};

}  // namespace nova::platform::vulkan
