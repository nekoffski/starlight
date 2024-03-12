#include "VKRendererBackend.h"

#include "VKBuffer.h"
#include "VKCommandBuffer.h"
#include "VKContext.h"
#include "VKDevice.h"
#include "VKFence.h"
#include "VKFramebuffer.h"
#include "VKImage.h"
#include "VKRenderPass.h"
#include "VKSwapchain.h"
#include "VKShader.h"

namespace sl::vk {

VKRendererBackend::VKRendererBackend(Window& window, const Config& config) :
    m_window(window), m_proxy(this), m_renderedVertices(0u) {
    const auto [w, h] = window.getSize();

    m_framebufferWidth  = w;
    m_framebufferHeight = h;

    createCoreComponents(window, config);
    createSemaphoresAndFences();
    createCommandBuffers();
}

VKRendererBackend::~VKRendererBackend() { m_device->waitIdle(); }

UniqPtr<VKUIRenderer> VKRendererBackend::createUIRendererer(RenderPass* renderPass) {
    return createUniqPtr<VKUIRenderer>(
      *m_context, *m_device, m_proxy, m_window, renderPass
    );
}

void VKRendererBackend::gpuCall(std::function<void(CommandBuffer& buffer)>&& callback
) {
    const auto queue = m_device->getQueues().graphics;

    vkQueueWaitIdle(queue);
    VKCommandBuffer commandBuffer(
      m_device.get(), m_device->getGraphicsCommandPool(),
      VKCommandBuffer::Severity::primary
    );
    commandBuffer.createAndBeginSingleUse();
    callback(commandBuffer);
    commandBuffer.endSingleUse(queue);
    m_device->waitIdle();
}

void VKRendererBackend::freeDataRange(
  VKBuffer& buffer, uint64_t offset, uint64_t size
) {
    buffer.free(size, offset);
}

void VKRendererBackend::drawMesh(const Mesh& mesh) {
    const auto& dataDescription = mesh.getDataDescription();
    auto& commandBuffer         = m_commandBuffers[m_imageIndex];

    VkDeviceSize offsets[1] = { dataDescription.vertexBufferOffset };

    vkCmdBindVertexBuffers(
      commandBuffer.getHandle(), 0, 1, m_objectVertexBuffer->getHandlePointer(),
      (VkDeviceSize*)offsets
    );
    if (dataDescription.indexCount > 0) {
        vkCmdBindIndexBuffer(
          commandBuffer.getHandle(), m_objectIndexBuffer->getHandle(),
          dataDescription.indexBufferOffset, VK_INDEX_TYPE_UINT32
        );
        vkCmdDrawIndexed(
          commandBuffer.getHandle(), dataDescription.indexCount, 1, 0, 0, 0
        );

        m_renderedVertices += dataDescription.indexCount;
    } else {
        vkCmdDraw(commandBuffer.getHandle(), dataDescription.vertexCount, 1, 0, 0);
        m_renderedVertices += dataDescription.vertexCount;
    }
}

void VKRendererBackend::createBuffers() {
    LOG_DEBUG("Creating buffers");

    m_objectVertexBuffer = createUniqPtr<VKBuffer>(
      m_context.get(), m_device.get(),
      VKBuffer::Properties{
        sizeof(Vertex3) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
          | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        true }
    );

    m_objectIndexBuffer = createUniqPtr<VKBuffer>(
      m_context.get(), m_device.get(),
      VKBuffer::Properties{
        sizeof(u32) * 1024 * 1024, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
          | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        true }
    );
}

VKCommandBuffer* VKRendererBackend::getCommandBuffer() {
    return &m_commandBuffers[m_imageIndex];
}

u32 VKRendererBackend::getImageIndex() { return m_imageIndex; }

void VKRendererBackend::createCoreComponents(
  sl::Window& window, const Config& config
) {
    m_context          = createUniqPtr<VKContext>(window, config);
    m_device           = createUniqPtr<VKDevice>(m_context.get());
    const auto& [w, h] = window.getSize();
    m_swapchain = createUniqPtr<VKSwapchain>(m_device.get(), m_context.get(), w, h);

    m_maxFramesInFlight = m_swapchain->getImageCount();

    createBuffers();

    m_resourcePools.emplace(
      *m_context, *m_device, *m_objectVertexBuffer, *m_objectIndexBuffer,
      *m_swapchain, this
    );
}

void VKRendererBackend::createSemaphoresAndFences() {
    m_imageAvailableSemaphores.reserve(m_maxFramesInFlight);
    m_queueCompleteSemaphores.reserve(m_maxFramesInFlight);

    m_imagesInFlight.resize(m_maxFramesInFlight);
    m_inFlightFences.reserve(m_maxFramesInFlight);

    auto contextPointer = m_context.get();
    auto devicePointer  = m_device.get();

    for (int i = 0; i < m_maxFramesInFlight; ++i) {
        m_imageAvailableSemaphores.emplace_back(contextPointer, devicePointer);
        m_queueCompleteSemaphores.emplace_back(contextPointer, devicePointer);
        m_inFlightFences.emplace_back(
          contextPointer, devicePointer, VKFence::State::signaled
        );
    }
}

void VKRendererBackend::onViewportResize(u32 width, u32 height) {
    m_framebufferWidth    = width;
    m_framebufferHeight   = height;
    m_recreatingSwapchain = true;
    LOG_TRACE("Vulkan renderer backend framebuffer resized {}/{}", width, height);

    auto logicalDevice = m_device->getLogicalDevice();
    if (auto result = vkDeviceWaitIdle(logicalDevice); not isGood(result)) {
        LOG_ERROR("vkDeviceWaitDile failed: %s", getResultString(result, true));
        // TODO: handle
    }
    m_inFlightFences[m_currentFrame].wait(UINT64_MAX);
    recreateSwapchain();

    m_recreatingSwapchain = false;
}

u64 VKRendererBackend::getRenderedVertexCount() const { return m_renderedVertices; }

void VKRendererBackend::setViewport(const Viewport& viewport) {
    setViewport(m_commandBuffers[m_imageIndex], viewport);
}

void VKRendererBackend::createCommandBuffers() {
    const auto swapchainImagesCount = m_swapchain->getImageCount();
    m_commandBuffers.clear();
    LOG_TRACE("Creating {} command buffers", swapchainImagesCount);
    m_commandBuffers.reserve(swapchainImagesCount);
    for (int i = 0; i < swapchainImagesCount; ++i) {
        m_commandBuffers.emplace_back(
          m_device.get(), m_device->getGraphicsCommandPool(),
          VKCommandBuffer::Severity::primary
        );
    }
}

void VKRendererBackend::recreateSwapchain() {
    if (auto result = m_device->waitIdle(); not isGood(result)) {
        LOG_ERROR("vkDeviceWaitIdle (2) failed: {}", getResultString(result, true));
        return;
    }

    // TODO: implement case when recreation fails
    m_swapchain->recreate(m_framebufferWidth, m_framebufferHeight);
    createCommandBuffers();
    LOG_INFO("Resized, booting.");
}

VKFence* VKRendererBackend::acquireImageFence() {
    auto& fence = m_imagesInFlight[m_imageIndex];
    if (fence) fence->wait(UINT64_MAX);
    fence = &m_inFlightFences[m_currentFrame];
    fence->reset();

    return fence;
}

Texture* VKRendererBackend::getFramebuffer(u64 id) {
    return m_swapchain->getFramebuffer(id);
}

Texture* VKRendererBackend::getDepthBuffer() {
    return m_swapchain->getDepthBuffer();
}

VKRendererBackendProxy* VKRendererBackend::getProxy() { return &m_proxy; }

void VKRendererBackend::setViewport(
  VKCommandBuffer& commandBuffer, const Viewport& viewport
) {
    VkViewport vkViewport;
    vkViewport.x        = viewport.x;
    vkViewport.y        = viewport.height;
    vkViewport.width    = viewport.width;
    vkViewport.height   = -viewport.height;
    vkViewport.minDepth = 0.0f;
    vkViewport.maxDepth = 1.0f;

    vkCmdSetViewport(commandBuffer.getHandle(), 0, 1, &vkViewport);
}

void VKRendererBackend::setScissors(VKCommandBuffer& commandBuffer) {
    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = m_framebufferWidth;
    scissor.extent.height               = m_framebufferHeight;

    vkCmdSetScissor(commandBuffer.getHandle(), 0, 1, &scissor);
}

bool VKRendererBackend::beginFrame(float deltaTime) {
    m_renderedVertices       = 0u;
    const auto logicalDevice = m_device->getLogicalDevice();

    if (m_recreatingSwapchain) {
        m_recreatingSwapchain = false;
        if (auto result = vkDeviceWaitIdle(logicalDevice); not isGood(result)) {
            LOG_ERROR("vkDeviceWaitDile failed: %s", getResultString(result, true));
            return false;
        }
        return false;
    }

    // Check if the framebuffer has been resized. If so, a new swapchain must be
    // created.

    // Wait for the execution of the current frame to complete. The fence being
    // free will allow this one to move on.
    if (not m_inFlightFences[m_currentFrame].wait(UINT64_MAX)) {
        LOG_WARN("In-flight fence wait failure!");
        return false;
    }

    // Acquire the next image from the swap chain. Pass along the semaphore that
    // should signaled when this completes. This same semaphore will later be
    // waited on by the queue submission to ensure this image is available.
    auto nextImageIndex = m_swapchain->acquireNextImageIndex(
      UINT64_MAX, m_imageAvailableSemaphores[m_currentFrame].getHandle(), nullptr
    );

    if (not nextImageIndex) return false;

    m_imageIndex = *nextImageIndex;

    // Begin recording commands.
    auto& commandBuffer = m_commandBuffers[m_imageIndex];

    commandBuffer.reset();
    commandBuffer.begin(VKCommandBuffer::BeginFlags{
      .isSingleUse          = false,
      .isRenderpassContinue = false,
      .isSimultaneousUse    = false,
    });

    setViewport(
      commandBuffer,
      Viewport{
        0.0f,
        0.0f,
        static_cast<float>(m_framebufferWidth),
        static_cast<float>(m_framebufferHeight),
      }
    );
    setScissors(commandBuffer);

    return true;
}

bool VKRendererBackend::endFrame(float deltaTime) {
    const auto logicalDevice = m_device->getLogicalDevice();
    auto& commandBuffer      = m_commandBuffers[m_imageIndex];

    commandBuffer.end();

    // Make sure the previous frame is not using this image (i.e. its fence is
    // being waited on) if (context.images_in_flight[context.image_index] !=
    // VK_NULL_HANDLE) {  // was frame vulkan_fence_wait(&context,
    // context.images_in_flight[context.image_index], UINT64_MAX);
    // }
    auto fence = acquireImageFence();

    // Submit the queue and wait for the operation to complete.
    // Begin queue submission
    VkSubmitInfo submit_info = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

    auto commandBufferHandle = commandBuffer.getHandle();

    // Command buffer(s) to be executed.
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers    = &commandBufferHandle;

    // The semaphore(s) to be signaled when the queue is complete.
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores =
      m_queueCompleteSemaphores[m_currentFrame].getHandlePointer();

    // Wait semaphore ensures that the operation cannot begin until the image is
    // available.
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores =
      m_imageAvailableSemaphores[m_currentFrame].getHandlePointer();

    // Each semaphore waits on the corresponding pipeline stage to complete. 1:1
    // ratio. VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT prevents subsequent
    // colour attachment writes from executing until the semaphore signals (i.e.
    // one frame is presented at atime)
    VkPipelineStageFlags flags[1] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    submit_info.pWaitDstStageMask = flags;

    const auto& deviceQueues = m_device->getQueues();

    VkResult result =
      vkQueueSubmit(deviceQueues.graphics, 1, &submit_info, fence->getHandle());

    if (result != VK_SUCCESS) {
        LOG_ERROR(
          "vkQueueSubmit failed with result: {}", getResultString(result, true)
        );
        return false;
    }

    commandBuffer.updateSubmitted();

    m_swapchain->present(
      deviceQueues.graphics, deviceQueues.present,
      m_queueCompleteSemaphores[m_currentFrame].getHandle(), m_imageIndex
    );

    m_currentFrame = (m_currentFrame + 1) % m_maxFramesInFlight;

    return true;
}

ResourcePools* VKRendererBackend::getResourcePools() {
    return m_resourcePools.get();
}

}  // namespace sl::vk
