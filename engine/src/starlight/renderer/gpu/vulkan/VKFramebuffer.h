#pragma once

#include <vector>

#include "starlight/core/math/Size.hpp"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class VKFramebuffer {
public:
    explicit VKFramebuffer(
      const VKContext* context, const VKDevice* device, VkRenderPass renderPass,
      const Size2u32& size, const std::vector<VkImageView>& attachments
    );

    VkFramebuffer getHandle();

    ~VKFramebuffer();

private:
    const VKContext* m_context;
    const VKDevice* m_device;

    std::vector<VkImageView> m_attachments;
    VkFramebuffer m_handle;
};

}  // namespace sl::vk
