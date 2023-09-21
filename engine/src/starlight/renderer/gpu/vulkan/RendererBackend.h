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

#include "GeometryData.h"
#include "MaterialShader.h"
#include "Pipeline.h"
#include "Semaphore.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "UIShader.h"

#include "Buffer.h"

namespace sl::vk {

class RendererBackend final : public sl::RendererBackend {
    struct FrameInfo {
        int frameCount = 0;
        uint32_t imageIndex = 0;
        uint32_t framebufferSizeGeneration = 0;
        uint32_t lastFramebufferSizeGeneration = 0;
        uint32_t currentFrame = 0;
    };

public:
    explicit RendererBackend(sl::Window& window, const Config& config);
    ~RendererBackend();

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

    virtual void acquireGeometryResources(Geometry& geometry,
                                          uint32_t vertexSize,
                                          uint32_t vertexCount,
                                          void* vertexData,
                                          std::span<uint32_t> indices) override;

    void releaseGeometryResources(Geometry& geometry) override;

private:
    void createCoreComponents(sl::Window& window, const Config& config);
    void createCommandBuffers();
    void regenerateFramebuffers();
    void createSemaphoresAndFences();

    uint64_t uploadDataRange(VkCommandPool pool, VkFence fence, VkQueue queue,
                             Buffer& outBuffer, uint64_t size,
                             const void* data);
    void freeDataRange(Buffer& buffer, uint64_t offset, uint64_t size);

    void createBuffers();

    void recreateSwapchain();
    void recordCommands(CommandBuffer& commandBuffer);

    FrameInfo m_frameInfo;

    Size2u32 m_framebufferSize;

    bool m_recreatingSwapchain = false;

    UniqPtr<Context> m_context;
    UniqPtr<Device> m_device;
    UniqPtr<Swapchain> m_swapchain;

    UniqPtr<RenderPass> m_mainRenderPass;
    UniqPtr<RenderPass> m_uiRenderPass;

    UniqPtr<TextureLoader> m_textureLoader;

    // TODO: consider creating as ptrs to allow mocking
    std::vector<CommandBuffer> m_commandBuffers;

    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_queueCompleteSemaphores;
    std::vector<Fence> m_inFlightFences;

    // one per frame
    std::vector<Fence*> m_imagesInFlight;

    UniqPtr<MaterialShader> m_materialShader;
    UniqPtr<UIShader> m_uiShader;

    UniqPtr<Buffer> m_objectVertexBuffer;
    UniqPtr<Buffer> m_objectIndexBuffer;

    static constexpr uint8_t maxFramesInFlight = 2;

    std::array<GeometryData, vulkanMaxGeometryCount> m_geometries;
    std::vector<Framebuffer> m_worldFramebuffers;
};

}  // namespace sl::vk