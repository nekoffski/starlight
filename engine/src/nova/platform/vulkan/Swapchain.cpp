// #include "Swapchain.h"

// #include <kc/core/Log.h>

// #include "Device.h"
// #include "Image.h"
// #include "Framebuffer.h"

// namespace nova::platform::vulkan {

// Swapchain::Swapchain(Device& device, Context& context, const math::Size2u32& size)
//     : device(device), context(context) {
//     create(size);
// }

// void Swapchain::create(const math::Size2u32& size) {
//     VkExtent2D swapchain_extent = {(uint32_t)size.width, (uint32_t)size.height};
//     // maxFramesInFlight           = 2;

//     // Choose a swap surface format.
//     bool found = false;

//     for (const auto& format : device.swapchain_support.formats) {
//         if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
//             format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
//             imageFormat = format;
//             found       = true;
//             break;
//         }
//     }

//     // Preferred formats
//     ASSERT(device.swapchain_support.formats.size() > 0, "Could not find formats for device");

//     if (not found) imageFormat = device.swapchain_support.formats[0];

//     VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
//     for (const auto& mode : device.swapchain_support.presentModes) {
//         if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
//             present_mode = mode;
//             break;
//         }
//     }

//     device.swapchain_support = queryDeviceSwapchainSupport(device.physicalDevice,
//     context.surface);

//     // Swapchain extent
//     if (device.swapchain_support.capabilities.currentExtent.width != UINT32_MAX) {
//         swapchain_extent = device.swapchain_support.capabilities.currentExtent;
//     }

//     // Clamp to the value allowed by the GPU.
//     VkExtent2D min          = device.swapchain_support.capabilities.minImageExtent;
//     VkExtent2D max          = device.swapchain_support.capabilities.maxImageExtent;
//     swapchain_extent.width  = std::clamp(swapchain_extent.width, min.width, max.width);
//     swapchain_extent.height = std::clamp(swapchain_extent.height, min.height, max.height);

//     uint32_t image_count = device.swapchain_support.capabilities.minImageCount + 1;
//     if (device.swapchain_support.capabilities.maxImageCount > 0 &&
//         image_count > device.swapchain_support.capabilities.maxImageCount) {
//         image_count = device.swapchain_support.capabilities.maxImageCount;
//     }

//     // Swapchain create info
//     VkSwapchainCreateInfoKHR swapchain_create_info =
//     {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR}; swapchain_create_info.surface =
//     context.surface; swapchain_create_info.minImageCount            = image_count;
//     swapchain_create_info.imageFormat              = imageFormat.format;
//     swapchain_create_info.imageColorSpace          = imageFormat.colorSpace;
//     swapchain_create_info.imageExtent              = swapchain_extent;
//     swapchain_create_info.imageArrayLayers         = 1;
//     swapchain_create_info.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

//     uint32_t queueFamilyIndices[] = {
//         (uint32_t)device.graphics_queue_index, (uint32_t)device.present_queue_index};

//     // Setup the queue family indices
//     if (device.graphics_queue_index != device.present_queue_index) {
//         swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
//         swapchain_create_info.queueFamilyIndexCount = 2;
//         swapchain_create_info.pQueueFamilyIndices   = queueFamilyIndices;
//     } else {
//         swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
//         swapchain_create_info.queueFamilyIndexCount = 0;
//         swapchain_create_info.pQueueFamilyIndices   = 0;
//     }

//     swapchain_create_info.preTransform   =
//     device.swapchain_support.capabilities.currentTransform; swapchain_create_info.compositeAlpha
//     = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; swapchain_create_info.presentMode    = present_mode;
//     swapchain_create_info.clipped        = VK_TRUE;
//     swapchain_create_info.oldSwapchain   = 0;

//     VK_CHECK(vkCreateSwapchainKHR(
//         device.logicalDevice, &swapchain_create_info, context.allocator, &handle
//     ));

//     // Images
//     uint32_t imageCount;
//     VK_CHECK(vkGetSwapchainImagesKHR(device.logicalDevice, handle, &imageCount, 0));

//     ASSERT(imageCount > 0, "imageCount==0 for vulkan swapchain");

//     images.resize(imageCount);
//     views.resize(imageCount);

//     VK_CHECK(vkGetSwapchainImagesKHR(device.logicalDevice, handle, &imageCount, images.data()));

//     // Views
//     for (uint32_t i = 0; i < imageCount; ++i) {
//         VkImageViewCreateInfo view_info           = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
//         view_info.image                           = images[i];
//         view_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
//         view_info.format                          = imageFormat.format;
//         view_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
//         view_info.subresourceRange.baseMipLevel   = 0;
//         view_info.subresourceRange.levelCount     = 1;
//         view_info.subresourceRange.baseArrayLayer = 0;
//         view_info.subresourceRange.layerCount     = 1;

//         VK_CHECK(vkCreateImageView(device.logicalDevice, &view_info, context.allocator,
//         &views[i]));
//     }

//     // Depth resources

//     if (!device.detectDepthFormat()) {
//         device.depthFormat = VK_FORMAT_UNDEFINED;
//         LOG_FATAL("Failed to find a supported format!");
//     }

//     // Create depth image and its view.
//     depthBuffer = std::make_unique<Image>(
//         device, context, VK_IMAGE_TYPE_2D,
//         math::Size2u32{swapchain_extent.width, swapchain_extent.height}, device.depthFormat,
//         VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
//         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, true, VK_IMAGE_ASPECT_DEPTH_BIT
//     );

//     LOG_INFO("Swapchain created successfully.");
// }

// Swapchain::~Swapchain() { destroy(); }

// void Swapchain::destroy() {
//     for (auto& imageView : views)
//         vkDestroyImageView(device.logicalDevice, imageView, context.allocator);
//     vkDestroySwapchainKHR(device.logicalDevice, handle, context.allocator);
// }

// std::optional<uint32_t> Swapchain::acquireNextImageIndex(
//     const Device& device, uint64_t timeoutNs, VkSemaphore imageSemaphore, VkFence fence,
//     const math::Size2u32& size
// ) {
//     uint32_t index;

//     VkResult result = vkAcquireNextImageKHR(
//         device.logicalDevice, handle, timeoutNs, imageSemaphore, fence, &index
//     );

//     if (result == VK_ERROR_OUT_OF_DATE_KHR) {
//         // Trigger swapchain recreation, then boot out of the render loop.
//         recreate(size);
//         return {};
//     } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
//         LOG_FATAL("Failed to acquire swapchain image!");
//         return {};
//     }

//     return index;
// }

// void Swapchain::present(
//     const Device& device, VkQueue graphics_queue, VkQueue present_queue,
//     VkSemaphore render_complete_semaphore, uint32_t present_image_index, const math::Size2u32&
//     size
// ) {
//     // Return the image to the swapchain for presentation.
//     VkPresentInfoKHR present_info   = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
//     present_info.waitSemaphoreCount = 1;
//     present_info.pWaitSemaphores    = &render_complete_semaphore;
//     present_info.swapchainCount     = 1;
//     present_info.pSwapchains        = &handle;
//     present_info.pImageIndices      = &present_image_index;
//     present_info.pResults           = 0;

//     VkResult result = vkQueuePresentKHR(present_queue, &present_info);
//     if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
//         // Swapchain is out of date, suboptimal or a framebuffer resize has occurred. Trigger
//         // swapchain recreation
//         recreate(size);
//     } else if (result != VK_SUCCESS) {
//         LOG_FATAL("Failed to present swap chain image!");
//     }
// }

// void Swapchain::recreate(const math::Size2u32& size) {
//     destroy();
//     create(size);
// }

// }  // namespace nova::platform::vulkan
