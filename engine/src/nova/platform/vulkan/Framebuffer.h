// #pragma once

// #include <vector>

// #include "nova/math/Size.hpp"

// #include "Vulkan.h"
// #include "fwd.h"

// namespace nova::platform::vulkan {

// struct Framebuffer {
//     explicit Framebuffer(
//         const Context& context, const Device& device, RenderPass* renderPass,
//         const math::Size2u32& size, const std::vector<VkImageView>& attachments
//     );

//     ~Framebuffer();

//     const Context& context;
//     const Device& device;

//     RenderPass* renderPass;
//     std::vector<VkImageView> attachments;
//     VkFramebuffer handle;
// };

// }  // namespace nova::platform::vulkan
