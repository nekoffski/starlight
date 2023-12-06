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

#include "VKGeometry.h"
#include "VKPipeline.h"
#include "VKSemaphore.h"
#include "VKTexture.h"
#include "VKBuffer.h"
#include "VKRendererContext.h"
#include "VKResourcePools.h"

namespace sl::vk {

class VKRendererBackend final : public RendererBackend {
public:
    explicit VKRendererBackend(sl::Window& window, const Config& config);
    ~VKRendererBackend();

    u32 getRenderPassId(const std::string& renderPass) const;

    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;

    bool beginRenderPass(uint8_t id);
    u64 endRenderPass(uint8_t id);

    void drawGeometry(const GeometryRenderData& model) override;

    void onViewportResize(uint32_t width, uint32_t height) override;

    void renderUI(std::function<void()>&&) override;

    void executeNow(
      VkQueue queue, std::function<void(VKCommandBuffer& buffer)>&& callback
    );

    // resources
    ResourcePools* getResourcePools() override;

    VKRenderPass* getRenderPass(u32 id);

private:
    void createCoreComponents(sl::Window& window, const Config& config);
    void createCommandBuffers();
    void createSemaphoresAndFences();

    void freeDataRange(VKBuffer& buffer, uint64_t offset, uint64_t size);

    void createBuffers();

    void initUI(Window& window);

    void recreateSwapchain();
    void recordCommands(VKCommandBuffer& commandBuffer);

    bool m_recreatingSwapchain = false;

    UniqPtr<VKContext> m_context;
    UniqPtr<VKDevice> m_device;
    UniqPtr<VKSwapchain> m_swapchain;

    VKRenderPass* m_mainRenderPass;
    VKRenderPass* m_uiRenderPass;

    LocalPtr<VKRendererContext> m_rendererContext;

    // TODO: consider creating as ptrs to allow mocking
    UniqPtr<VKBuffer> m_objectVertexBuffer;
    UniqPtr<VKBuffer> m_objectIndexBuffer;

    static constexpr uint8_t maxFramesInFlight = 2;

    // std::vector<VKRenderTarget> m_worldRenderTargets;

    VkDescriptorPool m_uiPool;

    u64 m_renderedVertices;

    LocalPtr<VKResourcePools> m_resourcePools;
};

}  // namespace sl::vk