#pragma once

#include <vector>
#include <cstdint>

#include "nova/gfx/RendererBacked.h"
#include "nova/core/fwd.h"
#include "nova/core/Window.h"
#include "nova/core/Config.h"
#include "nova/core/Memory.hpp"

#include "Vulkan.h"
#include "fwd.h"

#include "ShaderObject.h"
#include "Pipeline.h"
#include "Semaphore.h"

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

    void onViewportResize(uint32_t width, uint32_t height) override;

   private:
    void createCoreComponents(core::Window& window, const core::Config& config);
    void createCommandBuffers();
    void regenerateFramebuffers();
    void createSemaphoresAndFences();

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

    // TODO: consider creating as ptrs to allow mocking
    std::vector<CommandBuffer> m_commandBuffers;

    std::vector<Semaphore> m_imageAvailableSemaphores;
    std::vector<Semaphore> m_queueCompleteSemaphores;
    std::vector<Fence> m_inFlightFences;
    std::vector<Fence*> m_imagesInFlight;

    core::UniqPtr<ShaderObject> m_simpleShader;
    core::UniqPtr<Pipeline> m_pipeline;

    static constexpr uint8_t s_maxFramesInFlight = 2;
};

}  // namespace nova::platform::vulkan