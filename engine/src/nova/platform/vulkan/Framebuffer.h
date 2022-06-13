#pragma once

#include <vector>

#include "Vulkan.h"
#include "Device.h"
#include "RenderPass.h"

namespace nova::platform::vulkan {

namespace {

vk::raii::Framebuffer createFramebuffer(
    Device& device, RenderPass& renderPass, uint32_t width, uint32_t height,
    std::vector<vk::ImageView>& attachments
) {
    vk::FramebufferCreateInfo info{};

    info.renderPass      = *renderPass.handle;
    info.attachmentCount = attachments.size();
    info.pAttachments    = attachments.data();
    info.width           = width;
    info.height          = height;
    info.layers          = 1;

    return vk::raii::Framebuffer{*device.getLogicalDevice(), info};
}

}  // namespace

struct Framebuffer {
    explicit Framebuffer(
        Device& device, RenderPass& renderPass, uint32_t width, uint32_t height,
        const std::vector<vk::ImageView>& attachments
    )
        : renderPass(renderPass)
        , attachments(attachments)
        , handle(createFramebuffer(device, renderPass, width, height, this->attachments)) {}

    RenderPass& renderPass;
    std::vector<vk::ImageView> attachments;
    vk::raii::Framebuffer handle;
};

}  // namespace nova::platform::vulkan