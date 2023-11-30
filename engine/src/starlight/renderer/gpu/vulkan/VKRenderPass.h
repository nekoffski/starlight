#pragma once

#include "starlight/core/math/Glm.h"

#include "starlight/renderer/RenderPass.h"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class VKRenderPass : public RenderPass {
public:
    enum class State : unsigned char {
        ready,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    explicit VKRenderPass(
      u32 id, const VKContext* context, const VKDevice* device,
      const VKSwapchain& swapchain, const Properties& properties
    );

    ~VKRenderPass();

    void begin(VKCommandBuffer& commandBuffer, VkFramebuffer framebuffer);
    void end(VKCommandBuffer& commandBuffer);

    VkRenderPass getHandle();

private:
    std::vector<VkClearValue> createClearValues(u8 flags) const;

    VkRenderPassBeginInfo createRenderPassBeginInfo(
      const std::vector<VkClearValue>& clearValues, VkFramebuffer framebuffer
    ) const;

    const VKContext* m_context;
    const VKDevice* m_device;

    VkRenderPass m_handle;
    float m_depth      = 1.0f;
    uint32_t m_stencil = 0;

    State m_state;
};

}  // namespace sl::vk
