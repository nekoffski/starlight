#pragma once

#include <vector>

#include "nova/math/Size.hpp"

#include "Vulkan.h"
#include "fwd.h"

namespace nova::platform::vulkan {

class Framebuffer {
   public:
    struct Args {
        const Context* context;
        const Device* device;
        VkRenderPass renderPass;
        const math::Size2u32& size;
        const std::vector<VkImageView>& attachments;
    };

    explicit Framebuffer(const Args& args);

    VkFramebuffer getHandle();

    ~Framebuffer();

   private:
    const Context* m_context;
    const Device* m_device;

    std::vector<VkImageView> m_attachments;
    VkFramebuffer m_handle;
};

}  // namespace nova::platform::vulkan
