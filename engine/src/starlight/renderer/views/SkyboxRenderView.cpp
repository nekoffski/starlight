#include "SkyboxRenderView.h"

namespace sl {

SkyboxRenderView::SkyboxRenderView(
  Camera* camera, Shader* skyboxShader, Skybox* skybox
) :
    RenderView(camera),
    m_shader(skyboxShader), m_skybox(skybox) {}

void SkyboxRenderView::init(
  RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
  const InitProperties& initProperties
) {
    LOG_TRACE("Initializing SkyboxRenderView");
    auto backgroundColor = (1.0f / 255.0f) * glm::vec4{ 11, 16, 47, 255 };

    const auto w = initProperties.viewportWidth;
    const auto h = initProperties.viewportHeight;

    RenderPass::Properties renderPassProperties{
        .area            = glm::vec4{0.0f, 0.0f, w, h},
        .clearColor      = backgroundColor,
        .clearFlags      = RenderPass::clearColorBuffer,
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
    m_shader->createPipeline(m_renderPass);
    LOG_TRACE("SkyboxRenderView initialized");
}

void SkyboxRenderView::render(
  RendererBackendProxy& backendProxy, [[maybe_unused]] const RenderPacket& packet,
  [[maybe_unused]] const RenderProperties& properties,
  [[maybe_unused]] float deltaTime
) {
    m_renderPass->run(
      *backendProxy.getCommandBuffer(), backendProxy.getImageIndex(),
      [&]() {
          m_shader->use();
          m_shader->setGlobalUniforms([&](Shader::UniformProxy& proxy) {
              auto viewMatrix  = m_camera->getViewMatrix();
              viewMatrix[3][0] = 0.0f;
              viewMatrix[3][1] = 0.0f;
              viewMatrix[3][2] = 0.0f;

              proxy.set("view", viewMatrix);
              proxy.set("projection", m_camera->getProjectionMatrix());
          });
          m_shader->setInstanceUniforms(
            m_skybox->getInstanceId(),
            [&](Shader::UniformProxy& proxy) {
                proxy.set("cubeTexture", m_skybox->getCubeMap());
            }
          );
          backendProxy.drawMesh(*m_skybox->getMesh());
      }
    );
}

void SkyboxRenderView::onViewportResize(
  RendererBackendProxy& backendProxy, u32 w, u32 h
) {
    // TODO: make it generic
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
