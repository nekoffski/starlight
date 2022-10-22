#pragma once

#include "Vulkan.h"

#include "Device.h"
#include "Context.h"

namespace nova::platform::vulkan {

class Semaphore {
   public:
    explicit Semaphore(const Context* context, const Device* device)
        : m_context(context), m_device(device), m_handle(VK_NULL_HANDLE) {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

        VK_ASSERT(vkCreateSemaphore(
            m_device->getLogicalDevice(), &semaphoreCreateInfo, m_context->getAllocator(), &m_handle
        ));
    }

    ~Semaphore() {
        vkDestroySemaphore(m_device->getLogicalDevice(), m_handle, m_context->getAllocator());
    }

    VkSemaphore getHandle() const { return m_handle; }

    VkSemaphore* getHandlePointer() { return &m_handle; }

   private:
    const Context* m_context;
    const Device* m_device;

    VkSemaphore m_handle;
};

}  // namespace nova::platform::vulkan