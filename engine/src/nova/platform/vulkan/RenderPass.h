#pragma once

#include "nova/math/Glm.h"

#include "Vulkan.h"
#include "fwd.h"

namespace nova::platform::vulkan {

class RenderPass {
   public:
    enum class State : unsigned char {
        ready = 0,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    struct Args {
        const Context* context;
        const Device* device;
        const Swapchain& swapchain;
        const glm::vec4& area;
        const glm::vec4& color;
    };

    explicit RenderPass(const Args& args);

    ~RenderPass();

    void begin(CommandBuffer& commandBuffer, VkFramebuffer framebuffer);
    void end(CommandBuffer& commandBuffer);

    VkRenderPass getHandle();

    glm::vec4* getArea() { return &m_area; }

   private:
    std::vector<VkClearValue> createClearValues() const;

    VkRenderPassBeginInfo createRenderPassBeginInfo(
        const std::vector<VkClearValue>& clearValues, VkFramebuffer framebuffer
    ) const;

    const Context* m_context;
    const Device* m_device;

    VkRenderPass m_handle;
    glm::vec4 m_area;
    glm::vec4 m_color;

    float m_depth      = 1.0f;
    uint32_t m_stencil = 0;

    State m_state;
};

}  // namespace nova::platform::vulkan
