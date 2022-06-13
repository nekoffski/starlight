#pragma once

#include "Vulkan.h"

#include "nova/math/Glm.h"
#include "CommandBuffer.h"
#include "Device.h"

namespace nova::platform::vulkan {

namespace {

vk::raii::RenderPass createRenderPass(Device& device, const vk::SurfaceFormatKHR& swapchainFormat) {
    vk::SubpassDescription subpass{};
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;

    const uint32_t attachmentDescriptionCount = 2;
    vk::AttachmentDescription attachmentDescriptions[attachmentDescriptionCount];

    // color
    vk::AttachmentDescription colorAttachment;
    colorAttachment.format         = swapchainFormat.format;
    colorAttachment.samples        = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp         = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp        = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout  = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout    = vk::ImageLayout::ePresentSrcKHR;

    attachmentDescriptions[0] = colorAttachment;

    vk::AttachmentReference colorAttachmentReference{};
    colorAttachmentReference.attachment = 1;
    colorAttachmentReference.layout     = vk::ImageLayout::eColorAttachmentOptimal;

    // depth
    vk::AttachmentDescription depthAttachment;
    depthAttachment.format         = device.getDepthFormat();
    depthAttachment.samples        = vk::SampleCountFlagBits::e1;
    depthAttachment.loadOp         = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp        = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout  = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout    = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    attachmentDescriptions[1] = depthAttachment;

    vk::AttachmentReference depthAttachmentReference{};
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout     = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    subpass.pDepthStencilAttachment = &depthAttachmentReference;

    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments    = 0;

    subpass.pResolveAttachments     = 0;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments    = 0;

    vk::SubpassDependency dependency{};

    dependency.srcSubpass   = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass   = 0;
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    // dependency.srcAccessMask = 0;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask =
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

    vk::RenderPassCreateInfo info{};

    info.attachmentCount = attachmentDescriptionCount;
    info.pAttachments    = attachmentDescriptions;
    info.subpassCount    = 1;
    info.pSubpasses      = &subpass;
    info.dependencyCount = 1;
    info.pDependencies   = &dependency;
    info.pNext           = 0;

    return vk::raii::RenderPass{*device.getLogicalDevice(), info};
}

}  // namespace

struct RenderPass {
    enum class State : unsigned char {
        ready = 0,
        recording,
        inRenderPass,
        recordingEnded,
        submitted,
        notAllocated
    };

    explicit RenderPass(
        Device& device, const vk::SurfaceFormatKHR& swapchainFormat, const math::Vec4f& area,
        const math::Vec4f& backgroundColor, float depth, uint32_t stencil
    )
        : area(area)
        , backgroundColor(backgroundColor)
        , depth(depth)
        , stencil(stencil)
        , handle(createRenderPass(device, swapchainFormat)) {}

    void begin(CommandBuffer& commandBuffer, vk::raii::Framebuffer& framebuffer) {
        vk::RenderPassBeginInfo beginInfo{};

        beginInfo.renderPass               = *handle;
        beginInfo.framebuffer              = *framebuffer;
        beginInfo.renderArea.offset.x      = area.x;
        beginInfo.renderArea.offset.y      = area.y;
        beginInfo.renderArea.extent.width  = area.z;
        beginInfo.renderArea.extent.height = area.w;

        vk::ClearValue clearValues[2];

        for (int i = 0; i < 4; ++i) clearValues[0].color.float32[i] = backgroundColor[i];

        clearValues[1].depthStencil.depth   = depth;
        clearValues[1].depthStencil.stencil = stencil;

        beginInfo.clearValueCount = 2;
        beginInfo.pClearValues    = clearValues;

        commandBuffer.handle.beginRenderPass(beginInfo, vk::SubpassContents::eInline);
        commandBuffer.state = CommandBuffer::State::inRenderPass;
    }

    void end(CommandBuffer& commandBuffer) {
        commandBuffer.handle.endRenderPass();
        commandBuffer.state = CommandBuffer::State::recording;
    }

    vk::raii::RenderPass handle;

    math::Vec4f area;
    math::Vec4f backgroundColor;

    float depth;
    uint32_t stencil;

    State state;
};

}  // namespace nova::platform::vulkan
