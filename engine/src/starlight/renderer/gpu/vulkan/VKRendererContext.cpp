#include "VKRendererContext.h"

#include "VKDevice.h"

namespace sl::vk {

VKRendererContext::VKRendererContext(
  const u32 maxFramesInFlight, const Size2u32& framebufferSize
) :
    m_maxFramesInFlight(maxFramesInFlight),
    m_framebufferSize(framebufferSize) {}

u32 VKRendererContext::getImageIndex() const { return m_imageIndex; }
u32 VKRendererContext::getFramebufferSizeGeneration() const {
    return m_framebufferSizeGeneration;
}
u32 VKRendererContext::getLastFramebufferSizeGeneration() const {
    return m_lastFramebufferSizeGeneration;
}

VKCommandBuffer* VKRendererContext::getCommandBuffer() {
    return &m_commandBuffers[m_imageIndex];
}

const Size2u32& VKRendererContext::getFramebufferSize() const {
    return m_framebufferSize;
}

VKFence* VKRendererContext::acquireImageFence() {
    auto& fence = m_imagesInFlight[m_imageIndex];
    if (fence) fence->wait(UINT64_MAX);
    fence = &m_inFlightFences[m_currentFrame];
    fence->reset();

    return fence;
}

VKFence* VKRendererContext::getCurrentFence() {
    return &m_inFlightFences[m_currentFrame];
}

bool VKRendererContext::wasFramebufferResized() const {
    return m_framebufferSizeGeneration != m_lastFramebufferSizeGeneration;
}

void VKRendererContext::bumpFrameCounter() {
    m_currentFrame = (m_currentFrame + 1) % m_maxFramesInFlight;
}

void VKRendererContext::setImageIndex(u32 index) { m_imageIndex = index; }

void VKRendererContext::createSemaphoresAndFences(
  const VKContext* context, VKDevice* device
) {
    m_imageAvailableSemaphores.reserve(m_maxFramesInFlight);
    m_queueCompleteSemaphores.reserve(m_maxFramesInFlight);

    m_imagesInFlight.resize(m_maxFramesInFlight);
    m_inFlightFences.reserve(m_maxFramesInFlight);

    for (int i = 0; i < m_maxFramesInFlight; ++i) {
        m_imageAvailableSemaphores.emplace_back(context, device);
        m_queueCompleteSemaphores.emplace_back(context, device);
        m_inFlightFences.emplace_back(context, device, VKFence::State::signaled);
    }
}

void VKRendererContext::changeFramebufferSize(const Size2u32& size) {
    changeFramebufferSize(size.width, size.height);
}

VKSemaphore* VKRendererContext::getCurrentImageSemaphore() {
    return &m_imageAvailableSemaphores[m_currentFrame];
}

VKSemaphore* VKRendererContext::getCurrentQueueSemaphore() {
    return &m_queueCompleteSemaphores[m_currentFrame];
}

void VKRendererContext::changeFramebufferSize(u32 width, u32 height) {
    m_framebufferSize.width  = width;
    m_framebufferSize.height = height;

    m_lastFramebufferSizeGeneration = m_framebufferSizeGeneration;
    m_framebufferSizeGeneration++;

    LOG_TRACE(
      "Vulkan renderer backend framebuffer resized {}/{}/{}", width, height,
      m_framebufferSizeGeneration
    );
}

void VKRendererContext::createCommandBuffers(VKDevice* device, u32 count) {
    LOG_TRACE("Creating {} command buffers", count);
    m_commandBuffers.reserve(count);
    for (int i = 0; i < count; ++i) {
        m_commandBuffers.emplace_back(
          device, device->getGraphicsCommandPool(),
          VKCommandBuffer::Severity::primary
        );
    }
}

}  // namespace sl::vk
