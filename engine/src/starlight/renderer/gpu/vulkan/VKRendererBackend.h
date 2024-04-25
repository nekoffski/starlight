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

#include "VKMesh.h"
#include "VKPipeline.h"
#include "VKSemaphore.h"
#include "VKPhysicalDevice.h"
#include "VKLogicalDevice.h"
#include "VKTexture.h"
#include "VKBuffer.h"
#include "VKResourcePools.h"
#include "VKRendererBackendProxy.h"
#include "VKUIRenderer.h"
#include "VKBackendAccessor.h"

namespace sl::vk {

class VKRendererBackend : public RendererBackend, public VKBackendAccessor {
    friend class VKRendererBackendProxy;

public:
    explicit VKRendererBackend(sl::Window& window, const Config& config);
    ~VKRendererBackend();

    VKRendererBackend(const VKRendererBackend&)            = delete;
    VKRendererBackend& operator=(const VKRendererBackend&) = delete;
    VKRendererBackend(VKRendererBackend&&)                 = delete;
    VKRendererBackend& operator=(VKRendererBackend&&)      = delete;

    // backend accessor
    VKContext* getContext() override;
    VKLogicalDevice* getLogicalDevice() override;
    VKPhysicalDevice* getPhysicalDevice() override;

    // renderer backend

    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;

    void drawMesh(const Mesh& mesh) override;
    void onViewportResize(u32 width, u32 height) override;

    u64 getRenderedVertexCount() const override;
    void setViewport(const Viewport& viewport);

    // resources
    ResourcePools* getResourcePools() override;
    VKRendererBackendProxy* getProxy() override;

    void setViewport(VKCommandBuffer& commandBuffer, const Viewport& viewport);
    void setScissors(VKCommandBuffer& commandBuffer);

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

    VKFence* acquireImageFence();

    Texture* getFramebuffer(u64 id);
    Texture* getDepthBuffer();

    bool m_recreatingSwapchain;

    Window& m_window;

    VKContext m_context;
    VKPhysicalDevice m_physicalDevice;
    VKLogicalDevice m_logicalDevice;

    UniqPtr<VKSwapchain> m_swapchain;

    // vulkan

    // TODO: consider creating as ptrs to allow mocking
    UniqPtr<VKBuffer> m_objectVertexBuffer;
    UniqPtr<VKBuffer> m_objectIndexBuffer;

    VkDescriptorPool m_uiPool;

    u64 m_renderedVertices;

    LocalPtr<VKResourcePools> m_resourcePools;

    VKRendererBackendProxy m_proxy;

    u32 m_maxFramesInFlight;

    u32 m_imageIndex   = 0;
    u32 m_currentFrame = 0;

    u32 m_framebufferWidth;
    u32 m_framebufferHeight;

    std::vector<VKCommandBuffer> m_commandBuffers;

    std::vector<VKSemaphore> m_imageAvailableSemaphores;
    std::vector<VKSemaphore> m_queueCompleteSemaphores;
    std::vector<VKFence> m_inFlightFences;
    std::vector<VKFence*> m_imagesInFlight;
};

}  // namespace sl::vk
