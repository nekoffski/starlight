#include "WorldRenderView.h"

namespace sl {

WorldRenderView::WorldRenderView(Camera* camera, Shader* shader) :
    RenderView(camera), m_shader(shader) {}

void WorldRenderView::init(
  RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
  u32 viewportWidth, u32 viewportHeight
) {
    auto backgroundColor = (1.0f / 255.0f) * glm::vec4{ 11, 16, 47, 255 };

    RenderPass::Properties renderPassProperties{
        .area       = glm::vec4{0.0f, 0.0f, viewportWidth, viewportHeight},
        .clearColor = backgroundColor,
        .clearFlags =
          (RenderPass::clearColorBuffer | RenderPass::clearDepthBuffer
           | RenderPass::clearStencilBuffer),
        .hasPreviousPass = false,
        .hasNextPass     = false
    };

    RenderTarget::Properties renderTargetProperties{
        .attachments = {},
        .width       = viewportWidth,
        .height      = viewportHeight,
    };

    for (u8 i = 0; i < 3; ++i) {
        renderTargetProperties.attachments = {
            backendProxy.getFramebuffer(i), backendProxy.getDepthBuffer()
        };
        renderPassProperties.targets.push_back(renderTargetProperties);
    }

    m_renderPass = resourcePools.createRenderPass(renderPassProperties);
    m_shader->createPipeline(m_renderPass);
}

void WorldRenderView::render(
  RendererBackendProxy& backendProxy, const RenderPacket& renderPacket
) {
    m_renderPass->run(
      *backendProxy.getCommandBuffer(), backendProxy.getImageIndex(),
      [&]() {
          glm::vec4 ambientColor(0.3f, 0.3f, 0.3f, 1.0f);

          m_shader->use();
          m_shader->setGlobalUniforms([&](Shader::UniformProxy& proxy) {
              proxy.set("view", m_camera->getViewMatrix());
              proxy.set("projection", m_camera->getProjectionMatrix());
              proxy.set("viewPosition", m_camera->getPosition());
              proxy.set("ambientColor", ambientColor);
              proxy.set("renderMode", static_cast<int>(renderPacket.renderMode));
          });

          for (auto& mesh : renderPacket.meshes) {
              m_shader->setLocalUniforms([&](Shader::UniformProxy& proxy) {
                  proxy.set("model", mesh.transform.getWorld());
              });

              // TODO: shouldn't the material be bound to mesh instead of geometry?
              for (const auto& geometry : mesh.geometries) {
                  auto& material = geometry->getProperties().material;
                  material->applyUniforms(renderPacket.frameNumber);

                  backendProxy.drawGeometry(*geometry);
              }
          }
      }
    );
}

void WorldRenderView::onViewportResize(
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
        renderTargetProperties.attachments = {
            backendProxy.getFramebuffer(i), backendProxy.getDepthBuffer()
        };
        renderTargetsProperties.push_back(renderTargetProperties);
    }
    m_renderPass->regenerateRenderTargets(renderTargetsProperties);
}

}  // namespace sl