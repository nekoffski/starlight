#pragma once

#include "starlight/core/Core.h"
#include "starlight/core/math/Size.hpp"

#include "fwd.h"
#include "VKCommandBuffer.h"
#include "VKSemaphore.h"
#include "VKFence.h"
#include "VKDevice.h"

namespace sl::vk {

class VKRendererContext {
    friend class VKRendererBackend;

public:
    explicit VKRendererContext(
      const u32 maxFramesInFlight, const Size2u32& framebufferSize
    ) :
        m_maxFramesInFlight(maxFramesInFlight),
        m_framebufferSize(framebufferSize) {}

    u32 getImageIndex() const { return m_imageIndex; }
    u32 getFramebufferSizeGeneration() const { return m_framebufferSizeGeneration; }
    u32 getLastFramebufferSizeGeneration() const {
        return m_lastFramebufferSizeGeneration;
    }

    VKCommandBuffer* getCommandBuffer() { return &m_commandBuffers[m_imageIndex]; }

    const Size2u32& getFramebufferSize() const { return m_framebufferSize; }

private:
    VKFence* acquireImageFence() {
        auto& fence = m_imagesInFlight[m_imageIndex];
        if (fence) fence->wait(UINT64_MAX);
        fence = &m_inFlightFences[m_currentFrame];
        fence->reset();

        return fence;
    }

    VKFence* getCurrentFence() { return &m_inFlightFences[m_currentFrame]; }

    bool wasFramebufferResized() const {
        return m_framebufferSizeGeneration != m_lastFramebufferSizeGeneration;
    }

    void bumpFrameCounter() {
        m_currentFrame = (m_currentFrame + 1) % m_maxFramesInFlight;
    }

    void setImageIndex(u32 index) { m_imageIndex = index; }

    void createSemaphoresAndFences(const VKContext* context, VKDevice* device) {
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

    void changeFramebufferSize(const Size2u32& size) {
        changeFramebufferSize(size.width, size.height);
    }

    VKSemaphore* getCurrentImageSemaphore() {
        return &m_imageAvailableSemaphores[m_currentFrame];
    }

    VKSemaphore* getCurrentQueueSemaphore() {
        return &m_queueCompleteSemaphores[m_currentFrame];
    }

    void changeFramebufferSize(u32 width, u32 height) {
        m_framebufferSize.width  = width;
        m_framebufferSize.height = height;

        m_lastFramebufferSizeGeneration = m_framebufferSizeGeneration;
        m_framebufferSizeGeneration++;

        LOG_TRACE(
          "Vulkan renderer backend framebuffer resized {}/{}/{}", width, height,
          m_framebufferSizeGeneration
        );
    }

    void createCommandBuffers(VKDevice* device, u32 count) {
        LOG_TRACE("Creating {} command buffers", count);
        m_commandBuffers.reserve(count);
        for (int i = 0; i < count; ++i) {
            m_commandBuffers.emplace_back(
              device, device->getGraphicsCommandPool(),
              VKCommandBuffer::Severity::primary
            );
        }
    }

    u32 m_maxFramesInFlight;

    u32 m_imageIndex                    = 0;
    u32 m_framebufferSizeGeneration     = 0;
    u32 m_lastFramebufferSizeGeneration = 0;
    u32 m_currentFrame                  = 0;

    Size2u32 m_framebufferSize;

    std::vector<VKCommandBuffer> m_commandBuffers;

    std::vector<VKSemaphore> m_imageAvailableSemaphores;
    std::vector<VKSemaphore> m_queueCompleteSemaphores;
    std::vector<VKFence> m_inFlightFences;
    std::vector<VKFence*> m_imagesInFlight;
};

}  // namespace sl::vk
