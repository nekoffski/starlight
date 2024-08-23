#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

#include "starlight/core/Core.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/fwd.hh"
#include "starlight/core/memory/Memory.hh"
#include "starlight/core/window/Window.hh"
#include "starlight/renderer/gpu/RendererBackend.hh"
#include "starlight/renderer/gpu/RendererBackendProxy.hh"

#include "Vulkan.hh"
#include "fwd.hh"

#include "starlight/renderer/gpu/Shader.hh"

#include "VKMesh.hh"
#include "VKPipeline.hh"
#include "VKSemaphore.hh"
#include "VKPhysicalDevice.hh"
#include "VKLogicalDevice.hh"
#include "VKTexture.hh"
#include "VKBuffer.hh"
#include "VKResourcePools.hh"
#include "VKRendererBackendProxy.hh"
#include "VKUIRenderer.hh"

namespace sl::vk {

class VKRendererBackend : public RendererBackend {
    friend class VKRendererBackendProxy;

public:
    explicit VKRendererBackend(sl::Window& window, const Config& config);
    ~VKRendererBackend();

    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;

    void drawMesh(const Mesh& mesh) override;
    void onViewportResize(const Vec2<u32>& viewportSize) override;

    u64 getRenderedVertexCount() const override;
    void setViewport(const Rect2<u32>& viewport);

    // resources
    ResourcePools* getResourcePools() override;
    VKRendererBackendProxy* getProxy() override;

    VKBuffer& getIndexBuffer();
    VKBuffer& getVertexBuffer();

    VKContext& getContext();
    VKLogicalDevice& getLogicalDevice();

    void setViewport(VKCommandBuffer& commandBuffer, const Rect2<u32>& viewport);
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
