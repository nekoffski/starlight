// #pragma once

// #include <vector>
// #include <optional>
// #include <memory>

// #include "nova/math/Size.hpp"

// #include "Vulkan.h"
// #include "fwd.h"

// namespace nova::platform::vulkan {

// struct Swapchain {
//     explicit Swapchain(Device& device, Context& context, const math::Size2u32& size);
//     ~Swapchain();

//     void create(const math::Size2u32& size);
//     void destroy();

//     std::optional<uint32_t> acquireNextImageIndex(
//         const Device& device, uint64_t timeoutNs, VkSemaphore imageSemaphore, VkFence fence,
//         const math::Size2u32& size
//     );

//     void present(
//         const Device& device, VkQueue graphics_queue, VkQueue present_queue,
//         VkSemaphore render_complete_semaphore, uint32_t present_image_index,
//         const math::Size2u32& size
//     );

//     void recreate(const math::Size2u32& size);

//     Device& device;
//     Context& context;

//     VkSurfaceFormatKHR imageFormat;
//     // uint8_t maxFramesInFlight;
//     VkSwapchainKHR handle;

//     std::vector<VkImage> images;
//     std::vector<VkImageView> views;

//     std::unique_ptr<Image> depthBuffer;

//     std::vector<Framebuffer> framebuffers;
// };

// }  // namespace nova::platform::vulkan
