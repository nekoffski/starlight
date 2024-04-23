#pragma once

#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/math/Size.hpp"

#include "VKPhysicalDevice.h"
#include "VKContext.h"
#include "VKBackendAccessor.h"
#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class VKFramebuffer {
public:
    explicit VKFramebuffer(
      VKBackendAccessor& backendAccessor, VkRenderPass renderPass, u32 width,
      u32 height, const std::vector<VkImageView>& attachments
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
