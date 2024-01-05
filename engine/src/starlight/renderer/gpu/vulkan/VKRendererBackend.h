#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

#include "starlight/core/Core.h"
#include "starlight/core/Config.h"
#include "starlight/core/fwd.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/Window.h"
#include "starlight/renderer/gpu/RendererBackend.h"
#include "starlight/renderer/gpu/RendererBackendProxy.h"

#include "Vulkan.h"
#include "fwd.h"

#include "starlight/renderer/gpu/Shader.h"

#include "VKGeometry.h"
#include "VKPipeline.h"
#include "VKSemaphore.h"
#include "VKTexture.h"
#include "VKBuffer.h"
#include "VKResourcePools.h"
#include "VKRendererBackendProxy.h"
#include "VKUIRenderer.h"

namespace sl::vk {

class VKRendererBackend final : public RendererBackend {
    friend class VKRendererBackendProxy;

public:
    explicit VKRendererBackend(sl::Window& window, const Config& config);
    ~VKRendererBackend();

    template <typename C>
    requires Callable<C> u64 renderFrame(float deltaTime, C&& callback) {
        if (beginFrame(deltaTime)) {
            callback();
            endFrame(deltaTime);
        }
        return m_renderedVertices;
    }

    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;

    void drawGeometry(const Geometry& geometry) override;
    void onViewportResize(u32 width, u32 height) override;

    // resources
    ResourcePools* getResourcePools() override;
    VKRendererBackendProxy* getProxy() override;

private:
    UniqPtr<VKUIRenderer> createUIRendererer(RenderPass* renderPass);
    void gpuCall(std::function<void(CommandBuffer&)>&& callback);

    VKCommandBuffer* getCommandBuffer();
    u32 getImageIndex();

    void createCoreComponents(sl::Window& window, const Config& config);
    void createCommandBuffers();
    void createSemaphoresAndFences();

    void freeDataRange(VKBuffer& buffer, uint64_t offset, uint64_t size);

    void createBuffers();

    void recreateSwapchain();
    void recordCommands(VKCommandBuffer& commandBuffer);

    bool wasFramebufferResized();
    VKFence* acquireImageFence();

    Texture* getFramebuffer(u64 id);
    Texture* getDepthBuffer();

    Window& m_window;

    VKRendererBackendProxy m_proxy;

    bool m_recreatingSwapchain = false;

    UniqPtr<VKContext> m_context;
    UniqPtr<VKDevice> m_device;
    UniqPtr<VKSwapchain> m_swapchain;

    // TODO: consider creating as ptrs to allow mocking
    UniqPtr<VKBuffer> m_objectVertexBuffer;
    UniqPtr<VKBuffer> m_objectIndexBuffer;

    // std::vector<VKRenderTarget> m_worldRenderTargets;

    VkDescriptorPool m_uiPool;

    u64 m_renderedVertices;

    LocalPtr<VKResourcePools> m_resourcePools;

    u32 m_maxFramesInFlight;

    u32 m_imageIndex                    = 0;
    u32 m_framebufferSizeGeneration     = 0;
    u32 m_lastFramebufferSizeGeneration = 0;
    u32 m_currentFrame                  = 0;

    u32 m_framebufferWidth;
    u32 m_framebufferHeight;

    std::vector<VKCommandBuffer> m_commandBuffers;

    std::vector<VKSemaphore> m_imageAvailableSemaphores;
    std::vector<VKSemaphore> m_queueCompleteSemaphores;
    std::vector<VKFence> m_inFlightFences;
    std::vector<VKFence*> m_imagesInFlight;
};

}  // namespace sl::vk