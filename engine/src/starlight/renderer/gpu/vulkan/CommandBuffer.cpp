#include "CommandBuffer.h"

#include "Device.h"
#include "Context.h"

namespace sl::vk {

CommandBuffer::CommandBuffer(const Device* device, VkCommandPool commandPool, Severity severity)
    : m_device(device), m_commandPool(commandPool) {
    create(severity);
}

CommandBuffer::~CommandBuffer() { destroy(); }

VkCommandBufferAllocateInfo CommandBuffer::createAllocateInfo(Severity severity) {
    VkCommandBufferAllocateInfo allocateInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

    const auto level = severity == Severity::primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY
                                                     : VK_COMMAND_BUFFER_LEVEL_SECONDARY;

    allocateInfo.commandPool        = m_commandPool;
    allocateInfo.level              = level;
    allocateInfo.commandBufferCount = 1;
    allocateInfo.pNext              = 0;

    return allocateInfo;
}

void CommandBuffer::create(Severity severity) {
    m_state = State::notAllocated;

    auto allocateInfo = createAllocateInfo(severity);
    VK_ASSERT(vkAllocateCommandBuffers(m_device->getLogicalDevice(), &allocateInfo, &m_handle));

    m_state = State::ready;
}

void CommandBuffer::destroy() {
    LOG_TRACE("Destroying command buffer");

    if (m_handle) {
        vkFreeCommandBuffers(m_device->getLogicalDevice(), m_commandPool, 1, &m_handle);
        m_handle = VK_NULL_HANDLE;
    }
    m_state = State::notAllocated;
}

void CommandBuffer::begin(const BeginFlags& flags) {
    auto beginInfo = createCommandBufferBeginInfo(flags);
    VK_ASSERT(vkBeginCommandBuffer(m_handle, &beginInfo));
    m_state = State::recording;
}

void CommandBuffer::end() {
    VK_ASSERT(vkEndCommandBuffer(m_handle));
    m_state = State::recordingEnded;
}

void CommandBuffer::updateSubmitted() { m_state = State::submitted; }

void CommandBuffer::reset() { m_state = State::ready; }

// TODO: doesn't make sense, redesign to a static factor method
void CommandBuffer::createAndBeginSingleUse() {
    create(Severity::primary);

    begin(BeginFlags{
        .isSingleUse          = true,
        .isRenderpassContinue = false,
        .isSimultaneousUse    = false,
    });
}

VkSubmitInfo CommandBuffer::createSubmitQueueInfo() const {
    VkSubmitInfo submitInfo       = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &m_handle;

    return submitInfo;
}

void CommandBuffer::setState(State state) { m_state = state; }

VkCommandBuffer CommandBuffer::getHandle() const { return m_handle; }

void CommandBuffer::endSingleUse(VkQueue queue) {
    // End the command buffer.
    end();

    // Submit the queue
    auto submitInfo = createSubmitQueueInfo();
    VK_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, 0));

    // Wait for it to finish
    VK_ASSERT(vkQueueWaitIdle(queue));

    // Free the command buffer.
    destroy();
}

VkCommandBufferBeginInfo CommandBuffer::createCommandBufferBeginInfo(const BeginFlags& flags
) const {
    VkCommandBufferBeginInfo beginInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

    beginInfo.flags = 0;

    if (flags.isSingleUse) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (flags.isRenderpassContinue)
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

    if (flags.isSimultaneousUse) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    return beginInfo;
}

}  // namespace sl::vk
