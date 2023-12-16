#include "UIRenderView.h"

#include "starlight/renderer/ui/UI.hpp"

// TEMP: hide
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "starlight/renderer/gpu/vulkan/VKCommandBuffer.h"

namespace sl {

UIRenderView::UIRenderView(Camera* camera, UICallback&& callback) :
    RenderView(camera), m_uiCallback(std::move(callback)) {}

void UIRenderView::init(
  RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
  u32 viewportWidth, u32 viewportHeight
) {
    RenderPass::Properties renderPassProperties{
        .area            = glm::vec4{0.0f, 0.0f, viewportWidth, viewportHeight},
        .clearColor      = glm::vec4(0.0f),
        .clearFlags      = RenderPass::clearNone,
        .hasPreviousPass = true,
        .hasNextPass     = false
    };

    RenderTarget::Properties renderTargetProperties{
        .attachments = {},
        .width       = viewportWidth,
        .height      = viewportHeight,
    };

    for (u8 i = 0; i < 3; ++i) {
        renderTargetProperties.attachments = { backendProxy.getFramebuffer(i) };
        renderPassProperties.targets.push_back(renderTargetProperties);
    }

    m_renderPass = resourcePools.createRenderPass(renderPassProperties);
}

void UIRenderView::render(
  RendererBackendProxy& backendProxy, const RenderPacket& renderPacket
) {
    auto commandBuffer = backendProxy.getCommandBuffer();
    m_renderPass->run(*commandBuffer, backendProxy.getImageIndex(), [&]() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_uiCallback();

        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(
          ImGui::GetDrawData(),
          static_cast<vk::VKCommandBuffer*>(commandBuffer)->getHandle()
        );
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
}

}  // namespace sl