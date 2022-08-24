#include "RendererBackend.h"

#include "Context.h"
#include "Device.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "CommandBuffer.h"
#include "Fence.h"
#include "Image.h"
#include "Framebuffer.h"

namespace nova::platform::vulkan {

RendererBackend::RendererBackend(core::Window& window, const core::Config& config)
    : m_framebufferSize(window.getSize()) {
    createCoreComponents(window, config);
    regenerateFramebuffers();
    createCommandBuffers();
    createSemaphoresAndFences();
}

void RendererBackend::createCoreComponents(core::Window& window, const core::Config& config) {
    m_context   = core::createUniqPtr<Context>(window, config);
    m_device    = core::createUniqPtr<Device>(m_context.get());
    m_swapchain = core::createUniqPtr<Swapchain>(m_device.get(), m_context.get(), window.getSize());

    const auto& [width, height] = window.getSize();

    RenderPass::Args renderPassArgs{
        .context   = m_context.get(),
        .device    = m_device.get(),
        .swapchain = *m_swapchain,
        .area      = glm::vec4{0.0f, 0.0f, width, height},
        .color     = glm::vec4{0.3,  0.5f, 0.7f,  1.0f  },
    };

    m_renderPass = core::createUniqPtr<RenderPass>(renderPassArgs);
}

void RendererBackend::createSemaphoresAndFences() {
    const auto logicalDevice = m_device->getLogicalDevice();
    const auto allocator     = m_context->getAllocator();

    m_imageAvailableSemaphores.resize(s_maxFramesInFlight, VK_NULL_HANDLE);
    m_queueCompleteSemaphores.resize(s_maxFramesInFlight, VK_NULL_HANDLE);
    m_imagesInFlight.resize(s_maxFramesInFlight);

    m_inFlightFences.reserve(s_maxFramesInFlight);

    const auto createSemaphore = [&](int index) -> void {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

        // TODO: create wrapper
        vkCreateSemaphore(
            logicalDevice, &semaphoreCreateInfo, allocator, &m_imageAvailableSemaphores[index]
        );

        vkCreateSemaphore(
            logicalDevice, &semaphoreCreateInfo, allocator, &m_queueCompleteSemaphores[index]
        );

        m_inFlightFences.emplace_back(m_context.get(), m_device.get(), Fence::State::signaled);
    };

    for (int i = 0; i < s_maxFramesInFlight; ++i) createSemaphore(i);
}

void RendererBackend::onViewportResize(uint32_t width, uint32_t height) {
    m_framebufferSize.width = width;
    m_framebufferSize.width = height;

    m_frameInfo.lastFramebufferSizeGeneration = m_frameInfo.framebufferSizeGeneration;
    m_frameInfo.framebufferSizeGeneration++;

    LOG_TRACE(
        "Vulkan renderer backend onViewportResize {}/{}/{}", width, height,
        m_frameInfo.framebufferSizeGeneration
    );
}

void RendererBackend::createCommandBuffers() {
    const auto swapchainImagesCount = m_swapchain->getImagesSize();
    LOG_TRACE("Creating {} command buffers", swapchainImagesCount);

    m_commandBuffers.reserve(swapchainImagesCount);

    for (int i = 0; i < swapchainImagesCount; ++i) {
        m_commandBuffers.emplace_back(
            m_device.get(), m_device->getGraphicsCommandPool(), CommandBuffer::Severity::primary
        );
    }
}

void RendererBackend::regenerateFramebuffers() {
    const auto swapchainImagesCount = m_swapchain->getImagesSize();
    auto framebuffers               = m_swapchain->getFramebuffers();
    auto depthBuffer                = m_swapchain->getDepthBuffer();

    framebuffers->clear();
    framebuffers->reserve(swapchainImagesCount);

    for (auto& view : *m_swapchain->getImageViews()) {
        uint32_t attachment_count            = 2;
        std::vector<VkImageView> attachments = {view, depthBuffer->getView()};

        framebuffers->emplace_back(Framebuffer::Args{
            m_context.get(),
            m_device.get(),
            m_renderPass->getHandle(),
            m_framebufferSize,
            attachments,
        });
    }
}

RendererBackend::~RendererBackend() {}

void RendererBackend::recreateSwapchain() {
    if (auto result = vkDeviceWaitIdle(m_device->getLogicalDevice()); not isGood(result)) {
        LOG_ERROR("vkDeviceWaitIdle (2) failed: {}", getResultString(result, true));
        return;
    }

    // TODO: implement case when recreation fails
    m_swapchain->recreate();
    LOG_INFO("Resized, booting.");
}

void RendererBackend::recordCommands(CommandBuffer& commandBuffer) {
    commandBuffer.reset();
    commandBuffer.begin(CommandBuffer::BeginArgs{
        .isSingleUse          = false,
        .isRenderpassContinue = false,
        .isSimultaneousUse    = false,
    });

    // Dynamic state
    VkViewport viewport;
    viewport.x        = 0.0f;
    viewport.y        = (float)m_framebufferSize.height;
    viewport.width    = (float)m_framebufferSize.width;
    viewport.height   = -(float)m_framebufferSize.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Scissor
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = m_framebufferSize.width;
    scissor.extent.height               = m_framebufferSize.height;

    vkCmdSetViewport(commandBuffer.getHandle(), 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer.getHandle(), 0, 1, &scissor);
}

bool RendererBackend::beginFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();

    if (m_recreatingSwapchain) {
        if (auto result = vkDeviceWaitIdle(logicalDevice); not isGood(result)) {
            LOG_ERROR("vkDeviceWaitDile failed: %s", getResultString(result, true));
            return false;
        }

        LOG_INFO("Recreating swapchain, booting");
        return false;
    }

    // Check if the framebuffer has been resized. If so, a new swapchain must be created.
    if (m_frameInfo.framebufferSizeGeneration != m_frameInfo.lastFramebufferSizeGeneration) {
        recreateSwapchain();
        return false;
    }

    // Wait for the execution of the current frame to complete. The fence being free will allow this
    // one to move on.
    if (not m_inFlightFences[m_frameInfo.currentFrame].wait(UINT64_MAX)) {
        LOG_WARN("In-flight fence wait failure!");
        return false;
    }

    // Acquire the next image from the swap chain. Pass along the semaphore that should signaled
    // when this completes. This same semaphore will later be waited on by the queue submission
    // to ensure this image is available.
    auto nextImageIndex = m_swapchain->acquireNextImageIndex(
        UINT64_MAX, m_imageAvailableSemaphores[m_frameInfo.currentFrame], nullptr
    );

    if (not nextImageIndex) return false;

    m_frameInfo.imageIndex = *nextImageIndex;

    // Begin recording commands.
    auto& commandBuffer = m_commandBuffers[m_frameInfo.imageIndex];
    recordCommands(commandBuffer);

    m_renderPass->getArea()->z = m_framebufferSize.width;
    m_renderPass->getArea()->w = m_framebufferSize.height;

    auto& framebuffer = m_swapchain->getFramebuffers()->at(m_frameInfo.imageIndex);
    m_renderPass->begin(commandBuffer, framebuffer.getHandle());

    return true;
}

bool RendererBackend::endFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();
    auto& commandBuffer      = m_commandBuffers[m_frameInfo.imageIndex];

    m_renderPass->end(commandBuffer);
    commandBuffer.end();

    if (m_imagesInFlight[m_frameInfo.imageIndex] != VK_NULL_HANDLE)
        m_imagesInFlight[m_frameInfo.imageIndex]->wait(UINT64_MAX);

    // Make sure the previous frame is not using this image (i.e. its fence is being waited on)
    // if (context.images_in_flight[context.image_index] != VK_NULL_HANDLE) {  // was frame
    // vulkan_fence_wait(&context, context.images_in_flight[context.image_index], UINT64_MAX);
    // }

    // Mark the image fence as in-use by this frame.
    m_imagesInFlight[m_frameInfo.imageIndex] = &m_inFlightFences[m_frameInfo.currentFrame];

    // Reset the fence for use on the next frame
    m_inFlightFences[m_frameInfo.currentFrame].reset();

    // Submit the queue and wait for the operation to complete.
    // Begin queue submission
    VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};

    auto commandBufferHandle = commandBuffer.getHandle();

    // Command buffer(s) to be executed.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers    = &commandBufferHandle;

    // The semaphore(s) to be signaled when the queue is complete.
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores    = &m_queueCompleteSemaphores[m_frameInfo.currentFrame];

    // Wait semaphore ensures that the operation cannot begin until the image is available.
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores    = &m_imageAvailableSemaphores[m_frameInfo.currentFrame];

    // Each semaphore waits on the corresponding pipeline stage to complete. 1:1 ratio.
    // VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT prevents subsequent colour attachment
    // writes from executing until the semaphore signals (i.e. one frame is presented at atime)
    VkPipelineStageFlags flags[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_info.pWaitDstStageMask = flags;

    auto deviceQueues = m_device->getQueues();

    VkResult result = vkQueueSubmit(
        deviceQueues->graphics, 1, &submit_info,
        m_inFlightFences[m_frameInfo.currentFrame].getHandle()
    );

    if (result != VK_SUCCESS) {
        LOG_ERROR("vkQueueSubmit failed with result: {}", getResultString(result, true));
        return false;
    }

    commandBuffer.updateSubmitted();

    m_swapchain->present(
        deviceQueues->graphics, deviceQueues->present,
        m_queueCompleteSemaphores[m_frameInfo.currentFrame], m_frameInfo.imageIndex
    );

    m_frameInfo.currentFrame = (m_frameInfo.currentFrame + 1) % s_maxFramesInFlight;

    return true;
}

}  // namespace nova::platform::vulkan
