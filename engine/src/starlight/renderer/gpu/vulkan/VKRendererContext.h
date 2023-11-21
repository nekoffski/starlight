#pragma once

#include "starlight/core/Core.h"
#include "starlight/core/math/Size.hpp"

#include "fwd.h"
#include "VKCommandBuffer.h"
#include "VKSemaphore.h"
#include "VKFence.h"

namespace sl::vk {

// TODO: this name should be changed to something more descriptive
class VKRendererContext {
    friend class VKRendererBackend;

public:
    explicit VKRendererContext(
      const u32 maxFramesInFlight, const Size2u32& framebufferSize
    );

    u32 getImageIndex() const;
    u32 getFramebufferSizeGeneration() const;
    u32 getLastFramebufferSizeGeneration() const;

    VKCommandBuffer* getCommandBuffer();
    const Size2u32& getFramebufferSize() const;

private:
    VKFence* acquireImageFence();
    VKFence* getCurrentFence();

    bool wasFramebufferResized() const;
    void bumpFrameCounter();

    void setImageIndex(u32 index);

    void createSemaphoresAndFences(const VKContext* context, VKDevice* device);
    void createCommandBuffers(VKDevice* device, u32 count);
    void changeFramebufferSize(const Size2u32& size);

    VKSemaphore* getCurrentImageSemaphore();
    VKSemaphore* getCurrentQueueSemaphore();

    void changeFramebufferSize(u32 width, u32 height);

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
