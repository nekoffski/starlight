#include "CommandBuffer.h"

#include "Device.h"
#include "Context.h"

namespace nova::platform::vulkan {

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
    vkFreeCommandBuffers(m_device->getLogicalDevice(), m_commandPool, 1, &m_handle);
    m_state = State::notAllocated;
}

void CommandBuffer::begin(const BeginArgs& args) {
    auto beginInfo = createCommandBufferBeginInfo(args);
    VK_ASSERT(vkBeginCommandBuffer(m_handle, &beginInfo));
    m_state = State::recording;
}

void CommandBuffer::end() {
    VK_ASSERT(vkEndCommandBuffer(m_handle));
    m_state = State::recordingEnded;
}

void CommandBuffer::updateSubmitted() { m_state = State::submitted; }

void CommandBuffer::reset() { m_state = State::ready; }

void CommandBuffer::createAndBeginSingleUse(const Device& device, const VkCommandPool& pool) {
    create(Severity::primary);

    begin(BeginArgs{
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

void CommandBuffer::endSingleUse(const Device& device, const VkCommandPool& pool, VkQueue queue) {
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

VkCommandBufferBeginInfo CommandBuffer::createCommandBufferBeginInfo(const BeginArgs& args) const {
    VkCommandBufferBeginInfo beginInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};

    beginInfo.flags = 0;

    if (args.isSingleUse) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (args.isRenderpassContinue)
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

    if (args.isSimultaneousUse) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    return beginInfo;
}

}  // namespace nova::platform::vulkan
