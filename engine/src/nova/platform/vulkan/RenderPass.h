// #pragma once

// #include "nova/math/Glm.h"

// #include "Vulkan.h"
// #include "fwd.h"

// namespace nova::platform::vulkan {

// struct RenderPass {
//     enum class State : unsigned char {
//         ready = 0,
//         recording,
//         inRenderPass,
//         recordingEnded,
//         submitted,
//         notAllocated
//     };

//     explicit RenderPass(
//         const Context& context, const Device& device, const Swapchain& swapchain,
//         const glm::vec4& area, const glm::vec4& color
//     );
//     ~RenderPass();

//     void begin(CommandBuffer& commandBuffer, const VkFramebuffer& framebuffer);
//     void end(CommandBuffer& commandBuffer);

//     const Context& context;
//     const Device& device;

//     VkRenderPass handle;
//     glm::vec4 area;
//     glm::vec4 color;

//     float depth      = 1.0f;
//     uint32_t stencil = 0;

//     State state;
// };

// }  // namespace nova::platform::vulkan
