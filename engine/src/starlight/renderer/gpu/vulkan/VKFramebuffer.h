#pragma once

#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/math/Size.hpp"

#include "VKLogicalDevice.h"
#include "VKContext.h"

#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class VKFramebuffer {
public:
    explicit VKFramebuffer(
      VKContext& context, VKLogicalDevice& device, VkRenderPass renderPass,
      u32 width, u32 height, const std::vector<VkImageView>& attachments
    );

    VkFramebuffer getHandle();

    ~VKFramebuffer();

private:
    VKContext& m_context;
    VKLogicalDevice& m_device;

    std::vector<VkImageView> m_attachments;
    VkFramebuffer m_handle;
};

}  // namespace sl::vk
