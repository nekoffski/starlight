#include "RenderPass.h"

#include "Device.h"
#include "CommandBuffer.h"
#include "Swapchain.h"
#include "Context.h"

namespace nova::platform::vulkan {

// TODO: refactor
struct RenderPassCreateInfo {
    explicit RenderPassCreateInfo(VkFormat depthFormat, VkSurfaceFormatKHR surfaceFormat) {
        createSubpass();
        createColorAttachment(surfaceFormat);
        createDepthAttachment(depthFormat);

        // Depth stencil data.
        subpass.pDepthStencilAttachment = &depthAttachmentReference;

        // Input from a shader
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments    = 0;

        // Attachments used for multisampling colour attachments
        subpass.pResolveAttachments = 0;

        // Attachments not used in this subpass, but must be preserved for the next.
        subpass.preserveAttachmentCount = 0;
        subpass.pPreserveAttachments    = 0;
        subpass.flags                   = 0;

        createRenderPassDependencies();
        createRenderPassCreateInfo();
    }

    //  TODO: ASSIGN STRUCTURE TYPES IF MISSING!
    void createSubpass() { subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; }

    void createColorAttachment(VkSurfaceFormatKHR surfaceFormat) {
        colorAttachment.format         = surfaceFormat.format;  // TODO: configurable
        colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        // Do not expect any particular layout before render pass starts.
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        colorAttachment.flags         = 0;

        attachmentDescriptions.push_back(colorAttachment);

        colorAttachmentReference.attachment = 0;  // Attachment description array index
        colorAttachmentReference.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &colorAttachmentReference;
    }

    void createDepthAttachment(VkFormat depthFormat) {
        depthAttachment.format         = depthFormat;
        depthAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        depthAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.flags          = 0;

        attachmentDescriptions.push_back(depthAttachment);

        // Depth attachment reference
        depthAttachmentReference.attachment = 1;
        depthAttachmentReference.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    void createRenderPassDependencies() {
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass    = 0;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependency.dependencyFlags = 0;
    }

    void createRenderPassCreateInfo() {
        handle.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        handle.attachmentCount = attachmentDescriptions.size();
        handle.pAttachments    = attachmentDescriptions.data();
        handle.subpassCount    = 1;
        handle.pSubpasses      = &subpass;
        handle.dependencyCount = 1;
        handle.pDependencies   = &dependency;
        handle.pNext           = 0;
        handle.flags           = 0;
    }

    VkRenderPassCreateInfo handle;
    VkSubpassDescription subpass;

    std::vector<VkAttachmentDescription> attachmentDescriptions;

    VkAttachmentDescription colorAttachment;
    VkAttachmentReference colorAttachmentReference;

    VkAttachmentDescription depthAttachment;
    VkAttachmentReference depthAttachmentReference;

    VkSubpassDependency dependency;
};

RenderPass::RenderPass(
    const Context* context, const Device* device, const Swapchain& swapchain,
    const Properties& properties
)
    : m_context(context), m_device(device), m_area(properties.area), m_color(properties.color) {
    RenderPassCreateInfo createInfo{m_device->getDepthFormat(), swapchain.getSurfaceFormat()};

    VK_ASSERT(vkCreateRenderPass(
        m_device->getLogicalDevice(), &createInfo.handle, m_context->getAllocator(), &m_handle
    ));
}

RenderPass::~RenderPass() {
    if (m_handle)
        vkDestroyRenderPass(m_device->getLogicalDevice(), m_handle, m_context->getAllocator());
}

VkRenderPass RenderPass::getHandle() { return m_handle; }

std::vector<VkClearValue> RenderPass::createClearValues() const {
    std::vector<VkClearValue> clearValues(2);

    clearValues[0].color.float32[0]     = m_color.r;
    clearValues[0].color.float32[1]     = m_color.g;
    clearValues[0].color.float32[2]     = m_color.b;
    clearValues[0].color.float32[3]     = m_color.a;
    clearValues[1].depthStencil.depth   = m_depth;
    clearValues[1].depthStencil.stencil = m_stencil;

    return clearValues;
}

VkRenderPassBeginInfo RenderPass::createRenderPassBeginInfo(
    const std::vector<VkClearValue>& clearValues, VkFramebuffer framebuffer
) const {
    VkRenderPassBeginInfo beginInfo    = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    beginInfo.renderPass               = m_handle;
    beginInfo.framebuffer              = framebuffer;
    beginInfo.renderArea.offset.x      = m_area.x;
    beginInfo.renderArea.offset.y      = m_area.y;
    beginInfo.renderArea.extent.width  = m_area.z;
    beginInfo.renderArea.extent.height = m_area.w;

    beginInfo.clearValueCount = clearValues.size();
    beginInfo.pClearValues    = clearValues.data();

    return beginInfo;
}

void RenderPass::begin(CommandBuffer& commandBuffer, VkFramebuffer framebuffer) {
    auto clearValues = createClearValues();
    auto beginInfo   = createRenderPassBeginInfo(clearValues, framebuffer);

    vkCmdBeginRenderPass(commandBuffer.getHandle(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    commandBuffer.setState(CommandBuffer::State::inRenderPass);
}

void RenderPass::end(CommandBuffer& commandBuffer) {
    vkCmdEndRenderPass(commandBuffer.getHandle());
    commandBuffer.setState(CommandBuffer::State::recording);
}

}  // namespace nova::platform::vulkan
