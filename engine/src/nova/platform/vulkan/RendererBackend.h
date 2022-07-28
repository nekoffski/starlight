#pragma once

#include <vector>

#include "nova/gfx/RendererBacked.h"
#include "nova/core/fwd.h"
#include "nova/core/Window.h"
#include "nova/core/Config.h"

#include "Vulkan.h"
#include "Context.h"
#include "Device.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "CommandBuffer.h"
#include "Fence.h"
#include "Image.h"
#include "Framebuffer.h"

namespace nova::platform::vulkan {

class RendererBackend : public gfx::RendererBackend {
   public:
    explicit RendererBackend(core::Window& window, const core::Config& config)
        : m_frameCount(0)
        , m_context(window, config)
        , m_device(&m_context)
        , m_swapchain(&m_device, &m_context, window.getSize())
    // , m_renderPass(
    //       m_context, m_device, m_swapchain, glm::vec4{0.0f, 0.0f, 1600.0f, 900.0f},
    //       glm::vec4{0.3, 0.5f, 0.7f, 1.0f}
    //   )
    {
        // regenerateFramebuffers();
        // createCommandBuffers();

        // m_imageAvailableSemaphores.resize(m_maxFramesInFlight, VK_NULL_HANDLE);
        // m_queueCompleteSemaphores.resize(m_maxFramesInFlight, VK_NULL_HANDLE);
        // m_inFlightFences.reserve(m_maxFramesInFlight);
        // m_imagesInFlight.reserve(m_maxFramesInFlight);

        // for (int i = 0; i < m_maxFramesInFlight; ++i) {
        //     VkSemaphoreCreateInfo semaphore_create_info =
        //     {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

        //     // TODO: create wrapper
        //     vkCreateSemaphore(
        //         m_device.logicalDevice, &semaphore_create_info, m_context.allocator,
        //         &m_imageAvailableSemaphores[i]
        //     );

        //     vkCreateSemaphore(
        //         m_device.logicalDevice, &semaphore_create_info, m_context.allocator,
        //         &m_queueCompleteSemaphores[i]
        //     );

        //     m_inFlightFences.emplace_back(m_context, m_device, true);
        // }
    }

    ~RendererBackend() {}

    bool beginFrame(float deltaTime) override {
        // auto& device = m_device.logicalDevice;

        // if (m_recreatingSwapchain) {
        //     if (auto result = vkDeviceWaitIdle(device); not isGood(result)) {
        //         LOG_ERROR("vkDeviceWaitDile failed: %s", getResultString(result, true));
        //         return false;
        //     }

        //     LOG_INFO("Recreating swapchain, booting");
        //     return false;
        // }

        // // Check if the framebuffer has been resized. If so, a new swapchain must be created.
        // if (m_framebufferSizeGeneration != m_lastFramebufferSizeGeneration) {
        //     if (auto result = vkDeviceWaitIdle(device); not isGood(result)) {
        //         LOG_ERROR("vkDeviceWaitIdle (2) failed: {}", getResultString(result, true));
        //         return false;
        //     }

        //     // If the swapchain recreation failed (because, for example, the window was
        //     minimized),
        //     // boot out before unsetting the flag.
        //     // TODO
        //     // if (not m_swapchain.recreate()) return false;

        //     LOG_INFO("Resized, booting.");
        //     return false;
        // }

        // // Wait for the execution of the current frame to complete. The fence being free will
        // allow

        //     // this one to move on.
        //     if (!m_inFlightFences[m_currentFrame].wait(UINT64_MAX)) {
        //     LOG_WARN("In-flight fence wait failure!");
        //     return false;
        // }

        // // Acquire the next image from the swap chain. Pass along the semaphore that should
        // signaled
        //     // when this completes. This same semaphore will later be waited on by the queue
        //     submission
        //     // to ensure this image is available.

        //     auto nextImageIndex = m_swapchain.acquireNextImageIndex(
        //         m_device, UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame], nullptr,
        //         m_framebufferSize
        //     );

        // if (not nextImageIndex) return false;

        // m_imageIndex = *nextImageIndex;

        // // Begin recording commands.
        // auto& commandBuffer = m_commandBuffers[m_imageIndex];

        // commandBuffer.reset();
        // commandBuffer.begin(false, false, false);

        // // Dynamic state
        // VkViewport viewport;
        // viewport.x        = 0.0f;
        // viewport.y        = (float)m_framebufferSize.height;
        // viewport.width    = (float)m_framebufferSize.width;
        // viewport.height   = -(float)m_framebufferSize.height;
        // viewport.minDepth = 0.0f;
        // viewport.maxDepth = 1.0f;

        // // Scissor
        // VkRect2D scissor;
        // scissor.offset.x = scissor.offset.y = 0;
        // scissor.extent.width                = m_framebufferSize.width;
        // scissor.extent.height               = m_framebufferSize.height;

        // vkCmdSetViewport(commandBuffer.handle, 0, 1, &viewport);
        // vkCmdSetScissor(commandBuffer.handle, 0, 1, &scissor);

        // m_renderPass.area.z = m_framebufferSize.width;
        // m_renderPass.area.w = m_framebufferSize.height;

        // m_renderPass.begin(commandBuffer, m_swapchain.framebuffers[m_imageIndex].handle);

        return true;
    }

    bool endFrame(float deltaTime) override {
        // auto& device        = m_device.logicalDevice;
        // auto& commandBuffer = m_commandBuffers[m_imageIndex];

        // m_renderPass.end(commandBuffer);
        // commandBuffer.end();

        // if (m_imagesInFlight[m_imageIndex] != VK_NULL_HANDLE)
        //     m_imagesInFlight[m_imageIndex]->wait(UINT64_MAX);

        // // vulkan_command_buffer_end(command_buffer);

        // // Make sure the previous frame is not using this image (i.e. its fence is being waited
        // on)
        // // if (context.images_in_flight[context.image_index] != VK_NULL_HANDLE) {  // was frame
        // // vulkan_fence_wait(&context, context.images_in_flight[context.image_index],
        // UINT64_MAX);
        // // }

        // // Mark the image fence as in-use by this frame.
        // m_imagesInFlight[m_imageIndex] = &m_inFlightFences[m_currentFrame];

        // // Reset the fence for use on the next frame
        // m_inFlightFences[m_currentFrame].reset();

        // // Submit the queue and wait for the operation to complete.
        // // Begin queue submission
        // VkSubmitInfo submit_info = {VK_STRUCTURE_TYPE_SUBMIT_INFO};

        // // Command buffer(s) to be executed.
        // submit_info.commandBufferCount = 1;
        // submit_info.pCommandBuffers    = &commandBuffer.handle;

        // // The semaphore(s) to be signaled when the queue is complete.
        // submit_info.signalSemaphoreCount = 1;
        // submit_info.pSignalSemaphores    = &m_queueCompleteSemaphores[m_currentFrame];

        // // Wait semaphore ensures that the operation cannot begin until the image is available.
        // submit_info.waitSemaphoreCount = 1;
        // submit_info.pWaitSemaphores    = &m_imageAvailableSemaphores[m_currentFrame];

        // // Each semaphore waits on the corresponding pipeline stage to complete. 1:1 ratio.
        // // VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT prevents subsequent colour attachment
        // // writes from executing until the semaphore signals (i.e. one frame is presented at a
        // time) VkPipelineStageFlags flags[1] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        // submit_info.pWaitDstStageMask       = flags;

        // VkResult result = vkQueueSubmit(
        //     m_device.graphicsQueue, 1, &submit_info, m_inFlightFences[m_currentFrame].handle
        // );

        // if (result != VK_SUCCESS) {
        //     LOG_ERROR("vkQueueSubmit failed with result: {}", getResultString(result, true));
        //     return false;
        // }

        // commandBuffer.update_submitted();
        // m_swapchain.present(
        //     m_device, m_device.graphicsQueue, m_device.presentQueue,
        //     m_queueCompleteSemaphores[m_currentFrame], m_imageIndex, m_framebufferSize
        // );

        // m_currentFrame = (m_currentFrame + 1) % m_maxFramesInFlight;

        return true;
    }

    void onViewportResize(uint32_t width, uint32_t height) override {
        // m_framebufferSize.width = width;
        // m_framebufferSize.width = height;

        // m_lastFramebufferSizeGeneration = m_framebufferSizeGeneration;
        // m_framebufferSizeGeneration++;

        // LOG_TRACE(
        //     "Vulkan renderer backend onViewportResize {}/{}/{}", width, height,
        //     m_framebufferSizeGeneration
        // );
    }

    void createCommandBuffers() {
        // const auto swapchainImagesCount = m_swapchain.images.size();
        // LOG_TRACE("Creating {} command buffers", swapchainImagesCount);

        // m_commandBuffers.reserve(swapchainImagesCount);

        // for (int i = 0; i < swapchainImagesCount; ++i)
        //     m_commandBuffers.emplace_back(m_device, m_device.graphicsCommandPool, true);
    }

    void regenerateFramebuffers() {
        // const auto swapchainImagesCount = m_swapchain.images.size();

        // m_swapchain.framebuffers.clear();
        // m_swapchain.framebuffers.reserve(swapchainImagesCount);

        // for (auto& view : m_swapchain.views) {
        //     uint32_t attachment_count            = 2;
        //     std::vector<VkImageView> attachments = {view, m_swapchain.depthBuffer->view};

        //     m_swapchain.framebuffers.emplace_back(
        //         m_context, m_device, &m_renderPass, m_framebufferSize, attachments
        //     );
        // }
    }

   private:
    int m_frameCount;

    uint32_t m_imageIndex                    = 0;
    uint32_t m_framebufferSizeGeneration     = 0;
    uint32_t m_lastFramebufferSizeGeneration = 0;
    uint32_t m_currentFrame                  = 0;
    math::Size2u32 m_framebufferSize         = {1600, 900};

    static constexpr uint8_t m_maxFramesInFlight = 2;

    bool m_recreatingSwapchain = false;

    Context m_context;
    Device m_device;
    Swapchain m_swapchain;
    // RenderPass m_renderPass;

    // std::vector<CommandBuffer> m_commandBuffers;

    // std::vector<VkSemaphore> m_imageAvailableSemaphores;
    // std::vector<VkSemaphore> m_queueCompleteSemaphores;
    // std::vector<Fence> m_inFlightFences;
    // std::vector<Fence*> m_imagesInFlight;
};

}  // namespace nova::platform::vulkan