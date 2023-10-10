#pragma once

#include "starlight/core/math/Glm.h"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class VKRenderPass {
public:
    using ClearFlag = uint8_t;

    static inline constexpr ClearFlag clearFlagNone          = 0x0;
    static inline constexpr ClearFlag clearFlagColorBuffer   = 0x1;
    static inline constexpr ClearFlag clearFlagDepthBuffer   = 0x2;
    static inline constexpr ClearFlag clearFlagStencilBuffer = 0x4;

    enum class State : unsigned char {
        ready,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    struct Properties {
        glm::vec4 area;
        glm::vec4 color;
        ClearFlag clearFlags;
        bool hasPreviousPass;
        bool hasNextPass;
    };

    explicit VKRenderPass(
      const VKContext* context, const VKDevice* device, const VKSwapchain& swapchain,
      const Properties& properties
    );

    ~VKRenderPass();

    void begin(VKCommandBuffer& commandBuffer, VkFramebuffer framebuffer);
    void end(VKCommandBuffer& commandBuffer);

    VkRenderPass getHandle();

    glm::vec4* getArea() { return &m_area; }

    void setAmbient(const glm::vec4& ambient) { m_color = ambient; }

private:
    std::vector<VkClearValue> createClearValues(VKRenderPass::ClearFlag flags) const;

    VkRenderPassBeginInfo createRenderPassBeginInfo(
      const std::vector<VkClearValue>& clearValues, VkFramebuffer framebuffer
    ) const;

    const VKContext* m_context;
    const VKDevice* m_device;

    VkRenderPass m_handle;
    glm::vec4 m_area;
    glm::vec4 m_color;

    float m_depth      = 1.0f;
    uint32_t m_stencil = 0;

    State m_state;

    ClearFlag m_clearFlags;
    bool m_hasPreviousPass;
    bool m_hasNextPass;
};

}  // namespace sl::vk
