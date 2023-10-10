#include "VKFramebuffer.h"

#include "VKContext.h"
#include "VKDevice.h"

namespace sl::vk {

VkFramebufferCreateInfo createFramebufferCreateInfo(
  std::vector<VkImageView>& attachments, VkRenderPass renderPass,
  const Size2u32& size
) {
    VkFramebufferCreateInfo createInfo = {
        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO
    };
    createInfo.renderPass      = renderPass;
    createInfo.attachmentCount = attachments.size();
    createInfo.pAttachments    = attachments.data();
    createInfo.width           = size.width;
    createInfo.height          = size.height;
    createInfo.layers          = 1;

    return createInfo;
}

VkFramebuffer VKFramebuffer::getHandle() { return m_handle; }

VKFramebuffer::VKFramebuffer(
  const VKContext* context, const VKDevice* device, VkRenderPass renderPass,
  const Size2u32& size, const std::vector<VkImageView>& attachments
) :
    m_context(context),
    m_device(device), m_attachments(attachments) {
    const auto createInfo =
      createFramebufferCreateInfo(m_attachments, renderPass, size);

    VK_ASSERT(vkCreateFramebuffer(
      m_device->getLogicalDevice(), &createInfo, m_context->getAllocator(), &m_handle
    ));
}

VKFramebuffer::~VKFramebuffer() {
    if (m_handle)
        vkDestroyFramebuffer(
          m_device->getLogicalDevice(), m_handle, m_context->getAllocator()
        );
}

}  // namespace sl::vk
