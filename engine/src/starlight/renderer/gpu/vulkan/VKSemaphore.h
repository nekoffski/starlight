#pragma once

#include "Vulkan.h"

#include "fwd.h"

namespace sl::vk {

class VKSemaphore {
public:
    explicit VKSemaphore(const VKContext* context, const VKDevice* device);
    ~VKSemaphore();

    VkSemaphore getHandle() const;
    VkSemaphore* getHandlePointer();

private:
    const VKContext* m_context;
    const VKDevice* m_device;

    VkSemaphore m_handle;
};

}  // namespace sl::vk
