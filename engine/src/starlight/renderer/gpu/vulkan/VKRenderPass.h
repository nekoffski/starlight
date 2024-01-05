#pragma once

#include "starlight/core/math/Glm.h"

#include "starlight/renderer/gpu/RenderPass.h"

#include "Vulkan.h"
#include "fwd.h"

#include "VKRenderTarget.h"

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
      u32 id, VKContext* context, VKDevice* device, const VKSwapchain& swapchain,
      const Properties& properties
    );

    ~VKRenderPass();

    void begin(CommandBuffer& commandBuffer, u8 attachmentIndex) override;
    void end(CommandBuffer& commandBuffer) override;

    void regenerateRenderTargets(const std::vector<RenderTarget::Properties>& targets
    ) override;

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

    std::vector<VKRenderTarget> m_renderTargets;
};

}  // namespace sl::vk
