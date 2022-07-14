// #include "Image.h"

// #include "Device.h"

// namespace nova::platform::vulkan {

// Image::Image(
//     const Device& device, const Context& context, VkImageType type, const math::Size2u32& size,
//     VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
//     VkMemoryPropertyFlags memoryFlags, bool createView, VkImageAspectFlags viewAspectFlags
// )
//     : size(size), device(device), context(context) {
//     // Creation info.
//     VkImageCreateInfo image_create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
//     image_create_info.imageType         = VK_IMAGE_TYPE_2D;
//     image_create_info.extent.width      = size.width;
//     image_create_info.extent.height     = size.height;
//     image_create_info.extent.depth      = 1;  // TODO: Support configurable depth.
//     image_create_info.mipLevels         = 4;  // TODO: Support mip mapping
//     image_create_info.arrayLayers       = 1;  // TODO: Support number of layers in the image.
//     image_create_info.format            = format;
//     image_create_info.tiling            = tiling;
//     image_create_info.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
//     image_create_info.usage             = usage;
//     image_create_info.samples     = VK_SAMPLE_COUNT_1_BIT;      // TODO: Configurable sample
//     count. image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;  // TODO: Configurable
//     sharing mode.

//     VK_CHECK(vkCreateImage(device.logicalDevice, &image_create_info, context.allocator,
//     &handle));

//     // Query memory requirements.
//     VkMemoryRequirements memory_requirements;
//     vkGetImageMemoryRequirements(device.logicalDevice, handle, &memory_requirements);

//     uint32_t memory_type = device.findMemoryIndex(memory_requirements.memoryTypeBits,
//     memoryFlags);

//     if (memory_type == -1) LOG_ERROR("Required memory type not found. Image not valid.");

//     // Allocate memory
//     VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
//     memory_allocate_info.allocationSize       = memory_requirements.size;
//     memory_allocate_info.memoryTypeIndex      = memory_type;

//     VK_CHECK(
//         vkAllocateMemory(device.logicalDevice, &memory_allocate_info, context.allocator, &memory)
//     );

//     // Bind the memory
//     VK_CHECK(vkBindImageMemory(device.logicalDevice, handle, memory, 0)
//     );  // TODO: configurable memory offset.

//     if (createView) {
//         VkImageViewCreateInfo view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
//         view_create_info.image                 = handle;
//         view_create_info.viewType              = VK_IMAGE_VIEW_TYPE_2D;  // TODO: Make
//         configurable. view_create_info.format                = format;
//         view_create_info.subresourceRange.aspectMask = viewAspectFlags;

//         view_create_info.subresourceRange.baseMipLevel   = 0;
//         view_create_info.subresourceRange.levelCount     = 1;
//         view_create_info.subresourceRange.baseArrayLayer = 0;
//         view_create_info.subresourceRange.layerCount     = 1;

//         VK_CHECK(
//             vkCreateImageView(device.logicalDevice, &view_create_info, context.allocator, &view)
//         );
//     }
// }

// Image::~Image() {
//     if (view) vkDestroyImageView(device.logicalDevice, view, context.allocator);
//     if (memory) vkFreeMemory(device.logicalDevice, memory, context.allocator);
//     if (handle) vkDestroyImage(device.logicalDevice, handle, context.allocator);
// }

// }  // namespace nova::platform::vulkan
