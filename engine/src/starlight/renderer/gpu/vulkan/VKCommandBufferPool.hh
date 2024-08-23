#pragma once

#include <vector>

#include "VKCommandBuffer.hh"
#include "VKLogicalDevice.hh"

#include "starlight/renderer/gpu/CommandBufferPool.hh"

namespace sl::vk {

class VKCommandBufferPool : public CommandBufferPool {
public:
    explicit VKCommandBufferPool(
      VKContext& context, VKLogicalDevice& device, u64 size
    ) :
        m_device(device),
        m_frameImageIndex(0u), m_size(size) {
        create();
    }

    // static void useImmediateCommandBuffer(
    //   const VKDevice& device,
    //   std::function<void(VKCommandBuffer& buffer)>&& callback, VkQueue queue
    // ) {
    //     vkQueueWaitIdle(queue);

    //     VKCommandBuffer commandBuffer(
    //       &device, device.getGraphicsCommandPool(),
    //       VKCommandBuffer::Severity::primary
    //     );

    //     commandBuffer.begin(CommandBuffer::BeginFlags::singleUse);
    //     callback(commandBuffer);
    //     commandBuffer.end();

    //     auto handle = commandBuffer.getHandle();

    //     VkSubmitInfo submitInfo       = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
    //     submitInfo.commandBufferCount = 1;
    //     submitInfo.pCommandBuffers    = &handle;

    //     VK_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, 0));
    //     VK_ASSERT(vkQueueWaitIdle(queue));
    // }

    void recreate() {
        m_commandBuffers.clear();
        create();
    }

    void setFrameImageIndex(u32 frameImageIndex) {
        m_frameImageIndex = frameImageIndex;
    }

    VKCommandBuffer* getFrameCommandBuffer() override {
        return &m_commandBuffers[m_frameImageIndex];
    }

private:
    void create() {
        m_commandBuffers.reserve(m_size);

        for (int i = 0; i < m_size; ++i) {
            m_commandBuffers.emplace_back(
              m_device, m_device.getGraphicsCommandPool()
            );
        }
    }

    VKLogicalDevice& m_device;

    u32 m_frameImageIndex;
    u64 m_size;

    std::vector<VKCommandBuffer> m_commandBuffers;
};

}  // namespace sl::vk