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

    RenderPass::Properties renderPassProperties{
        .rect       = Rect2u32{Vec2u32{ 0u, 0u }, initProperties.viewportSize},
        .clearColor = glm::vec4(0.0f),
        .clearFlags = RenderPass::clearNone,
        .hasPreviousPass = initProperties.hasPreviousView,
        .hasNextPass     = initProperties.hasNextView
    };

    RenderTarget::Properties renderTargetProperties{
        .attachments = {}, .size = initProperties.viewportSize
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
        m_uiRenderer->render(*commandBuffer, m_uiCallback);
    });
}

void UIRenderView::onViewportResize(
  RendererBackendProxy& backendProxy, Vec2u32 viewportSize
) {
    // TODO: get swapchain images count from backend
    std::vector<RenderTarget::Properties> renderTargetsProperties;
    renderTargetsProperties.reserve(3);

    RenderTarget::Properties renderTargetProperties{
        .attachments = {}, .size = viewportSize
    };

    for (u8 i = 0; i < 3; ++i) {
        renderTargetProperties.attachments = { backendProxy.getFramebuffer(i) };
        renderTargetsProperties.push_back(renderTargetProperties);
    }
    m_renderPass->regenerateRenderTargets(renderTargetsProperties);
    m_renderPass->setRectSize(viewportSize);
}

}  // namespace sl