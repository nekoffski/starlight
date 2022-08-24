#include "Fence.h"

#include "Device.h"
#include "Context.h"

namespace nova::platform::vulkan {

VkFenceCreateInfo createFenceCreateInfo(Fence::State state) {
    VkFenceCreateInfo fenceCreateInfo = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};

    if (state == Fence::State::signaled) fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    return fenceCreateInfo;
}

Fence::Fence(const Context* context, const Device* device, State state)
    : m_context(context), m_device(device), m_state(state) {
    auto fenceCreateInfo = createFenceCreateInfo(state);
    VK_ASSERT(vkCreateFence(
        m_device->getLogicalDevice(), &fenceCreateInfo, m_context->getAllocator(), &m_handle
    ));
}

Fence::~Fence() {
    if (m_handle) vkDestroyFence(m_device->getLogicalDevice(), m_handle, m_context->getAllocator());
}

VkFence Fence::getHandle() { return m_handle; }

void logError(VkResult result) {
    switch (result) {
        case VK_TIMEOUT:
            LOG_WARN("vk_fence_wait - Timed out");
            break;
        case VK_ERROR_DEVICE_LOST:
            LOG_ERROR("vk_fence_wait - VK_ERROR_DEVICE_LOST.");
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            LOG_ERROR("vk_fence_wait - VK_ERROR_OUT_OF_HOST_MEMORY.");
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            LOG_ERROR("vk_fence_wait - VK_ERROR_OUT_OF_DEVICE_MEMORY.");
            break;
        default:
            LOG_ERROR("vk_fence_wait - An unknown error has occurred.");
            break;
    }
}

bool Fence::wait(Nanoseconds timeout) {
    if (m_state == State::signaled) return true;

    const auto result = vkWaitForFences(m_device->getLogicalDevice(), 1, &m_handle, true, timeout);

    if (result == VK_SUCCESS) {
        m_state = State::signaled;
        return true;
    }

    logError(result);
    return false;
}

void Fence::reset() {
    if (m_state == State::signaled) {
        VK_ASSERT(vkResetFences(m_device->getLogicalDevice(), 1, &m_handle));
        m_state = State::notSignaled;
    }
}

}  // namespace nova::platform::vulkan
