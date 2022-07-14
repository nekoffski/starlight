// #include "Framebuffer.h"

// #include "Context.h"
// #include "Device.h"
// #include "RenderPass.h"

// namespace nova::platform::vulkan {

// Framebuffer::Framebuffer(
//     const Context& context, const Device& device, RenderPass* renderPass,
//     const math::Size2u32& size, const std::vector<VkImageView>& attachments
// )
//     : context(context), device(device), renderPass(renderPass), attachments(attachments) {
//     // Creation info
//     VkFramebufferCreateInfo framebuffer_create_info =
//     {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO}; framebuffer_create_info.renderPass =
//     renderPass->handle; framebuffer_create_info.attachmentCount         = attachments.size();
//     framebuffer_create_info.pAttachments            = this->attachments.data();
//     framebuffer_create_info.width                   = size.width;
//     framebuffer_create_info.height                  = size.height;
//     framebuffer_create_info.layers                  = 1;

//     VK_CHECK(vkCreateFramebuffer(
//         device.logicalDevice, &framebuffer_create_info, context.allocator, &handle
//     ));
// }

// Framebuffer::~Framebuffer() {
//     if (handle) vkDestroyFramebuffer(device.logicalDevice, handle, context.allocator);
// }

// }  // namespace nova::platform::vulkan
