#include "Framebuffer.h"

#include "Context.h"
#include "Device.h"

namespace nova::platform::vulkan {

VkFramebufferCreateInfo createFramebufferCreateInfo(
    std::vector<VkImageView>& attachments, VkRenderPass renderPass, const math::Size2u32& size
) {
    VkFramebufferCreateInfo createInfo = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    createInfo.renderPass              = renderPass;
    createInfo.attachmentCount         = attachments.size();
    createInfo.pAttachments            = attachments.data();
    createInfo.width                   = size.width;
    createInfo.height                  = size.height;
    createInfo.layers                  = 1;

    return createInfo;
}

Framebuffer::Framebuffer(const Args& args)
    : m_context(args.context), m_device(args.device), m_attachments(args.attachments) {
    const auto createInfo = createFramebufferCreateInfo(m_attachments, args.renderPass, args.size);

    VK_ASSERT(vkCreateFramebuffer(
        m_device->getLogicalDevice(), &createInfo, m_context->getAllocator(), &m_handle
    ));
}

Framebuffer::~Framebuffer() {
    if (m_handle)
        vkDestroyFramebuffer(m_device->getLogicalDevice(), m_handle, m_context->getAllocator());
}

}  // namespace nova::platform::vulkan
