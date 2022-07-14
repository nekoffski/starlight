// #pragma once

// #include <optional>

// #include "nova/math/Size.hpp"

// #include "Vulkan.h"
// #include "fwd.h"

// namespace nova::platform::vulkan {

// struct Image {
//     explicit Image(
//         const Device& device, const Context& context, VkImageType type, const math::Size2u32&
//         size, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
//         VkMemoryPropertyFlags memoryFlags, bool createView, VkImageAspectFlags viewAspectFlags
//     );
//     ~Image();

//     VkImage handle;
//     VkDeviceMemory memory;
//     VkImageView view;
//     math::Size2u32 size;

//     const Device& device;
//     const Context& context;
// };

// }  // namespace nova::platform::vulkan
