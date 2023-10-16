#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

#include "starlight/core/Config.h"
#include "starlight/core/fwd.h"
#include "starlight/core/memory/Memory.hpp"
#include "starlight/core/window/Window.h"
#include "starlight/renderer/gpu/RendererBackend.h"

#include "Vulkan.h"
#include "fwd.h"

#include "starlight/renderer/Shader.h"

#include "VKGeometryData.h"
#include "VKPipeline.h"
#include "VKSemaphore.h"
#include "VKTexture.h"
#include "VKBuffer.h"
#include "VKTextureLoader.h"
#include "VKRendererContext.h"

namespace sl::vk {

class VKRendererBackend final : public RendererBackend {
public:
    explicit VKRendererBackend(sl::Window& window, const Config& config);
    ~VKRendererBackend();

    std::unique_ptr<Shader::Impl> createShaderImpl(sl::Shader& shader) override;

    u32 getRenderPassId(const std::string& renderPass) const override;

    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;

    bool beginRenderPass(uint8_t id);
    bool endRenderPass(uint8_t id);

    void drawGeometry(const GeometryRenderData& model) override;

    void onViewportResize(uint32_t width, uint32_t height) override;

    TextureLoader* getTextureLoader() const override;

    virtual void acquireGeometryResources(
      Geometry& geometry, uint32_t vertexSize, uint32_t vertexCount,
      void* vertexData, std::span<uint32_t> indices
    ) override;

    void releaseGeometryResources(Geometry& geometry) override;

private:
    void createCoreComponents(sl::Window& window, const Config& config);
    void createCommandBuffers();
    void regenerateFramebuffers();
    void createSemaphoresAndFences();

    uint64_t uploadDataRange(
      VkCommandPool pool, VkFence fence, VkQueue queue, VKBuffer& outBuffer,
      uint64_t size, const void* data
    );
    void freeDataRange(VKBuffer& buffer, uint64_t offset, uint64_t size);

    void createBuffers();

    void recreateSwapchain();
    void recordCommands(VKCommandBuffer& commandBuffer);

    VKRenderPass* getRenderPass(u32 id);

    bool m_recreatingSwapchain = false;

    UniqPtr<VKContext> m_context;
    UniqPtr<VKDevice> m_device;
    UniqPtr<VKSwapchain> m_swapchain;

    UniqPtr<VKRenderPass> m_mainRenderPass;
    UniqPtr<VKRenderPass> m_uiRenderPass;

    UniqPtr<VKTextureLoader> m_textureLoader;

    VKRendererContext m_rendererContext;

    // TODO: consider creating as ptrs to allow mocking
    UniqPtr<VKBuffer> m_objectVertexBuffer;
    UniqPtr<VKBuffer> m_objectIndexBuffer;

    static constexpr uint8_t maxFramesInFlight = 2;

    std::array<VKGeometryData, vulkanMaxGeometryCount> m_geometries;
    std::vector<VKFramebuffer> m_worldFramebuffers;
};

}  // namespace sl::vk