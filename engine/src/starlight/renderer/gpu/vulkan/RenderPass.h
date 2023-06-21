#pragma once

#include "starlight/core/math/Glm.h"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

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

    struct Properties {
        const glm::vec4& area;
        const glm::vec4& color;
    };

    explicit RenderPass(
        const Context* context, const Device* device, const Swapchain& swapchain,
        const Properties& properties
    );

    ~RenderPass();

    void begin(CommandBuffer& commandBuffer, VkFramebuffer framebuffer);
    void end(CommandBuffer& commandBuffer);

    VkRenderPass getHandle();

    glm::vec4* getArea() { return &m_area; }

    void setAmbient(const glm::vec4& ambient) { m_color = ambient; }

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

}  // namespace sl::vk
