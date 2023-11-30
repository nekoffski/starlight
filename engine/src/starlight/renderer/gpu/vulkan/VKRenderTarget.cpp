#include "VKRenderTarget.h"

#include "VKRenderPass.h"
#include "VKTexture.h"

namespace sl::vk {

VKRenderTarget::VKRenderTarget(
  u32 id, VKContext& context, VKDevice& device, const Properties& props
) :
    RenderTarget(id, props),
    m_context(context), m_device(device) {
    regenerate(props);
}

void VKRenderTarget::regenerate(const Properties& properties) {
    if (m_framebuffer) m_framebuffer.clear();

    std::vector<VkImageView> attachmentViews;
    attachmentViews.reserve(m_props.attachments.size());

    for (auto& attachment : m_props.attachments) {
        attachmentViews.push_back(
          static_cast<VKTexture*>(attachment)->getImage()->getView()
        );
    }

    // m_framebuffer.emplace(
    //   &m_context, &m_device,
    //   dynamic_cast<VKRenderPass*>(m_props.renderPass)->getHandle(), m_props.width,
    //   m_props.height, {}
    // );
}

}  // namespace sl::vk
