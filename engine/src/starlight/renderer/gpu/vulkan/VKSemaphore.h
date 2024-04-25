#pragma once

#include "Vulkan.h"

#include "VKPhysicalDevice.h"
#include "VKContext.h"
#include "VKBackendAccessor.h"

#include "fwd.h"

namespace sl::vk {

class VKSemaphore {
public:
    explicit VKSemaphore(VKBackendAccessor& backendAccssor);
    ~VKSemaphore();

    VkSemaphore getHandle() const;
    VkSemaphore* getHandlePointer();

private:
    VKContext& m_context;
    VKLogicalDevice& m_device;

    VkSemaphore m_handle;
};

}  // namespace sl::vk
