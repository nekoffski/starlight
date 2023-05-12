#pragma once

#include <vector>
#include <array>
#include <span>
#include <cstdint>

#include "nova/gfx/RendererBackend.h"
#include "nova/core/fwd.h"
#include "nova/core/Window.h"
#include "nova/core/Config.h"
#include "nova/core/Memory.hpp"

#include "Vulkan.h"
#include "fwd.h"

#include "MaterialShader.h"
#include "Pipeline.h"
#include "Semaphore.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "GeometryData.h"

#include "Buffer.h"

namespace nova::platform::vulkan {

class RendererBackend : public gfx::RendererBackend {
    struct FrameInfo {
        int frameCount                         = 0;
        uint32_t imageIndex                    = 0;
        uint32_t framebufferSizeGeneration     = 0;
        uint32_t lastFramebufferSizeGeneration = 0;
        uint32_t currentFrame                  = 0;
    };

   public:
    explicit RendererBackend(core::Window& window, const core::Config& config);
    ~RendererBackend();

    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;

    void updateGlobalState(const gfx::GlobalState& globalState) override;
    void drawGeometry(const gfx::GeometryRenderData& model) override;

    void onViewportResize(uint32_t width, uint32_t height) override;

    TextureLoader* getTextureLoader() const override;

    void acquireMaterialResources(gfx::Material& material) override;
    void releaseMaterialResources(gfx::Material& material) override;

    void acquireGeometryResources(
        gfx::Geometry& geometry, std::span<math::Vertex3> vertices, std::span<uint32_t> indices
    ) override;
    void releaseGeometryResources(gfx::Geometry& geometry) override;

   private:
    void createCoreComponents(core::Window& window, const core::Config& config);
    void createCommandBuffers();
    void regenerateFramebuffers();
    void createSemaphoresAndFences();

    void uploadDataRange(
        VkCommandPool pool, VkFence fence, VkQueue queue, Buffer& outBuffer, uint64_t offset,
        uint64_t size, const void* data
    );

    void createBuffers();

    void recreateSwapchain();
    void recordCommands(CommandBuffer& commandBuffer);

    void createPipeline();

    FrameInfo m_frameInfo;

    math::Size2u32 m_framebufferSize;

    bool m_recreatingSwapchain = false;

    core::UniqPtr<Context> m_context;
    core::UniqPtr<Device> m_device;
    core::UniqPtr<Swapchain> m_swapchain;
    core::UniqPtr<RenderPass> m_renderPass;
    core::UniqPtr<TextureLoader> m_textureLoader;

    // TODO: consider creating as ptrs to allow mocking
    std::vector<CommandBuffer> m_commandBuffers;

    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_queueCompleteSemaphores;
    std::vector<Fence> m_inFlightFences;
    std::vector<Fence*> m_imagesInFlight;

    core::UniqPtr<MaterialShader> m_simpleShader;
    core::UniqPtr<Pipeline> m_pipeline;

    core::UniqPtr<Buffer> m_objectVertexBuffer;
    core::UniqPtr<Buffer> m_objectIndexBuffer;

    static constexpr uint8_t maxFramesInFlight = 2;

    std::array<GeometryData, vulkanMaxGeometryCount> m_geometries;

    uint32_t m_geometryVertexOffset = 0;
    uint32_t m_geometryIndexOffset  = 0;
};

}  // namespace nova::platform::vulkan