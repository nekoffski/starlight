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
#include "VKMaterialShader.h"
#include "VKPipeline.h"
#include "VKSemaphore.h"
#include "VKTexture.h"
#include "VKUIShader.h"
#include "VKBuffer.h"
#include "VKTextureLoader.h"

namespace sl::vk {

class VKRendererBackend final : public RendererBackend {
    struct FrameInfo {
        int frameCount                         = 0;
        uint32_t imageIndex                    = 0;
        uint32_t framebufferSizeGeneration     = 0;
        uint32_t lastFramebufferSizeGeneration = 0;
        uint32_t currentFrame                  = 0;
    };

public:
    explicit VKRendererBackend(sl::Window& window, const Config& config);
    ~VKRendererBackend();

    std::unique_ptr<Shader::Impl> createShaderImpl(sl::Shader& shader) override;

    u32 getRenderPassId(const std::string& renderPass) const override;

    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;

    void updateGlobalWorldState(const GlobalState& globalState) override;
    void updateGlobalUIState(Mat4f projection, Mat4f view, int32_t mode);

    bool beginRenderPass(uint8_t id);
    bool endRenderPass(uint8_t id);

    void drawGeometry(const GeometryRenderData& model) override;

    void onViewportResize(uint32_t width, uint32_t height) override;

    TextureLoader* getTextureLoader() const override;

    void acquireMaterialResources(Material& material) override;
    void releaseMaterialResources(Material& material) override;

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

    FrameInfo m_frameInfo;

    Size2u32 m_framebufferSize;

    bool m_recreatingSwapchain = false;

    UniqPtr<VKContext> m_context;
    UniqPtr<VKDevice> m_device;
    UniqPtr<VKSwapchain> m_swapchain;

    UniqPtr<VKRenderPass> m_mainRenderPass;
    UniqPtr<VKRenderPass> m_uiRenderPass;

    UniqPtr<VKTextureLoader> m_textureLoader;

    // TODO: consider creating as ptrs to allow mocking
    std::vector<VKCommandBuffer> m_commandBuffers;

    std::vector<VKSemaphore> m_imageAvailableSemaphores;
    std::vector<VKSemaphore> m_queueCompleteSemaphores;
    std::vector<VKFence> m_inFlightFences;

    // one per frame
    std::vector<VKFence*> m_imagesInFlight;

    UniqPtr<VKMaterialShader> m_materialShader;
    UniqPtr<VKUIShader> m_uiShader;

    UniqPtr<VKBuffer> m_objectVertexBuffer;
    UniqPtr<VKBuffer> m_objectIndexBuffer;

    static constexpr uint8_t maxFramesInFlight = 2;

    std::array<VKGeometryData, vulkanMaxGeometryCount> m_geometries;
    std::vector<VKFramebuffer> m_worldFramebuffers;
};

}  // namespace sl::vk