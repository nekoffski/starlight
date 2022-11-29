#include "Semaphore.h"

#include "Device.h"
#include "Context.h"

namespace nova::platform::vulkan {

Semaphore::Semaphore(const Context* context, const Device* device)
    : m_context(context), m_device(device), m_handle(VK_NULL_HANDLE) {
    VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    VK_ASSERT(vkCreateSemaphore(
        m_device->getLogicalDevice(), &semaphoreCreateInfo, m_context->getAllocator(), &m_handle
    ));
}

Semaphore::~Semaphore() {
    if (m_handle)
        vkDestroySemaphore(m_device->getLogicalDevice(), m_handle, m_context->getAllocator());
}

VkSemaphore Semaphore::getHandle() const { return m_handle; }

VkSemaphore* Semaphore::getHandlePointer() { return &m_handle; }

}  // namespace nova::platform::vulkan
