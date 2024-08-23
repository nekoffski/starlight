#pragma once

#include "Vulkan.h"

#include "starlight/renderer/gpu/CommandBuffer.h"

#include "VKLogicalDevice.h"
#include "fwd.h"

namespace sl::vk {

class VKCommandBuffer : public CommandBuffer {
public:
    explicit VKCommandBuffer(
      VKLogicalDevice& device, VkCommandPool commandPool,
      Severity severity = Severity::primary
    );

    ~VKCommandBuffer();

    VkCommandBufferAllocateInfo createAllocateInfo(Severity severity);

    void create(Severity severity);
    void destroy();

    void begin(const BeginFlags& args) override;
    void end() override;

    void updateSubmitted();
    void reset();

    void setState(State state);

    VkCommandBuffer getHandle() const;

    void createAndBeginSingleUse();
    void endSingleUse(VkQueue queue);

private:
    VkSubmitInfo createSubmitQueueInfo() const;
    VkCommandBufferBeginInfo createCommandBufferBeginInfo(const BeginFlags& args
    ) const;

    VKLogicalDevice& m_device;

    VkCommandPool m_commandPool;
    VkCommandBuffer m_handle;
    State m_state;
};
}  // namespace sl::vk
