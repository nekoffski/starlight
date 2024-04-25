#pragma once

#include "Vulkan.h"

#include "VKLogicalDevice.h"
#include "VKContext.h"

#include "fwd.h"

namespace sl::vk {

class VKSemaphore {
public:
    explicit VKSemaphore(VKContext& context, VKLogicalDevice& device);
    ~VKSemaphore();

    VkSemaphore getHandle() const;
    VkSemaphore* getHandlePointer();

private:
    VKContext& m_context;
    VKLogicalDevice& m_device;

    VkSemaphore m_handle;
};

}  // namespace sl::vk
