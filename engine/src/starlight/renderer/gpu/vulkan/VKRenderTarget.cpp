#include "VKRenderTarget.hh"

#include "VKRenderPass.hh"
#include "VKTexture.hh"

namespace sl::vk {

VKRenderTarget::VKRenderTarget(
  u32 id, VKContext& context, VKLogicalDevice& device, VKRenderPass* renderPass,
  const Properties& props
) :
    RenderTarget(id, props),
    m_context(context), m_device(device), m_renderPass(renderPass) {
    VKRenderTarget::regenerate(props);
}

VKFramebuffer* VKRenderTarget::getFramebuffer() { return m_framebuffer.get(); }

void VKRenderTarget::regenerate(const Properties& properties) {
    m_props = properties;

    if (m_framebuffer) m_framebuffer.clear();

    std::vector<VkImageView> attachmentViews;
    attachmentViews.reserve(m_props.attachments.size());

    for (auto& attachment : m_props.attachments) {
        attachmentViews.push_back(
          static_cast<VKTexture*>(attachment)->getImage()->getView()
        );
    }

    m_framebuffer.emplace(
      m_context, m_device, m_renderPass->getHandle(), m_props.size.x, m_props.size.y,
      attachmentViews
    );
}

}  // namespace sl::vk
