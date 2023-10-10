#include "VKSemaphore.h"

#include "VKDevice.h"
#include "VKContext.h"

namespace sl::vk {

VKSemaphore::VKSemaphore(const VKContext* context, const VKDevice* device) :
    m_context(context), m_device(device), m_handle(VK_NULL_HANDLE) {
    VkSemaphoreCreateInfo semaphoreCreateInfo = {
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VK_ASSERT(vkCreateSemaphore(
      m_device->getLogicalDevice(), &semaphoreCreateInfo, m_context->getAllocator(),
      &m_handle
    ));
}

VKSemaphore::~VKSemaphore() {
    if (m_handle)
        vkDestroySemaphore(
          m_device->getLogicalDevice(), m_handle, m_context->getAllocator()
        );
}

VkSemaphore VKSemaphore::getHandle() const { return m_handle; }

VkSemaphore* VKSemaphore::getHandlePointer() { return &m_handle; }

}  // namespace sl::vk
