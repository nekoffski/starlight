#pragma once

#include "starlight/core/window/Window.h"

#include "starlight/renderer/gpu/UIRenderer.h"

#include "VKCommandBuffer.h"
#include "VKRendererBackendProxy.h"
#include "Vulkan.h"
#include "fwd.h"

namespace sl::vk {

class VKUIRenderer : public UIRenderer {
public:
    explicit VKUIRenderer(
      VKContext& context, VKDevice& device, RendererBackendProxy& backendProxy,
      Window& window, RenderPass* renderPass
    );

    ~VKUIRenderer() override;

    void reloadFontTextures() override;

private:
    void begin(CommandBuffer& commandBuffer) override;
    void end(CommandBuffer& commandBuffer) override;

    VKContext& m_context;
    VKDevice& m_device;
    RendererBackendProxy& m_backendProxy;

    VkDescriptorPool m_uiPool;

    // allow only single instance per engine
    inline static bool s_hasInstance = false;
};

}  // namespace sl::vk
