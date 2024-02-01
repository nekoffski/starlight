#include "UIRenderView.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "starlight/renderer/gpu/vulkan/VKCommandBuffer.h"

namespace sl {

UIRenderView::UIRenderView(
  Camera* camera, const Properties& props, UICallback&& callback
) :
    RenderView(camera),
    m_props(props), m_uiCallback(std::move(callback)) {}

void UIRenderView::init(
  RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
  const InitProperties& initProperties
) {
    LOG_TRACE("Initializing UIRenderView");
    const auto w = initProperties.viewportWidth;
    const auto h = initProperties.viewportHeight;

    RenderPass::Properties renderPassProperties{
        .area            = glm::vec4{0.0f, 0.0f, w, h},
        .clearColor      = glm::vec4(0.0f),
        .clearFlags      = RenderPass::clearNone,
        .hasPreviousPass = initProperties.hasPreviousView,
        .hasNextPass     = initProperties.hasNextView
    };

    RenderTarget::Properties renderTargetProperties{
        .attachments = {},
        .width       = w,
        .height      = h,
    };

    for (u8 i = 0; i < 3; ++i) {
        renderTargetProperties.attachments = { backendProxy.getFramebuffer(i) };
        renderPassProperties.targets.push_back(renderTargetProperties);
    }

    m_renderPass = resourcePools.createRenderPass(renderPassProperties);
    m_uiRenderer = backendProxy.createUIRendererer(m_renderPass);

    m_uiRenderer->setStyle();

    for (const auto& fontProperties : m_props.fonts)
        m_fonts.push_back(m_uiRenderer->addFont(fontProperties));

    LOG_TRACE("UIRenderView initialized");
}

void UIRenderView::render(
  RendererBackendProxy& backendProxy, [[maybe_unused]] const RenderPacket& packet,
  [[maybe_unused]] const RenderProperties& properties, float deltaTime
) {
    auto commandBuffer = backendProxy.getCommandBuffer();
    m_renderPass->run(*commandBuffer, backendProxy.getImageIndex(), [&]() {
        m_uiRenderer->render(*commandBuffer, m_uiCallback, deltaTime);
    });
}

void UIRenderView::onViewportResize(
  RendererBackendProxy& backendProxy, u32 w, u32 h
) {
    // TODO: get swapchain images count from backend
    std::vector<RenderTarget::Properties> renderTargetsProperties;
    renderTargetsProperties.reserve(3);

    RenderTarget::Properties renderTargetProperties{
        .attachments = {},
        .width       = w,
        .height      = h,
    };

    for (u8 i = 0; i < 3; ++i) {
        renderTargetProperties.attachments = { backendProxy.getFramebuffer(i) };
        renderTargetsProperties.push_back(renderTargetProperties);
    }
    m_renderPass->regenerateRenderTargets(renderTargetsProperties);
    m_renderPass->setArea(glm::vec4{ 0.0f, 0.0f, w, h });
}

}  // namespace sl