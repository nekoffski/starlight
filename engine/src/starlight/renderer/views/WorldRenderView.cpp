#include "WorldRenderView.h"

namespace sl {

WorldRenderView::WorldRenderView(Camera* camera, Shader* shader) :
    RenderView(camera), m_shader(shader) {}

void WorldRenderView::init(
  RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
  const InitProperties& initProperties
) {
    LOG_TRACE("Initializing WorldRenderView");
    auto backgroundColor = (1.0f / 255.0f) * glm::vec4{ 11, 16, 47, 255 };

    const auto w = initProperties.viewportWidth;
    const auto h = initProperties.viewportHeight;

    u8 clearFlags = RenderPass::clearDepthBuffer | RenderPass::clearStencilBuffer;
    if (not initProperties.hasPreviousView)
        clearFlags |= RenderPass::clearColorBuffer;

    RenderPass::Properties renderPassProperties{
        .area            = glm::vec4{0.0f, 0.0f, w, h},
        .clearColor      = backgroundColor,
        .clearFlags      = clearFlags,
        .hasPreviousPass = initProperties.hasPreviousView,
        .hasNextPass     = initProperties.hasNextView
    };

    RenderTarget::Properties renderTargetProperties{
        .attachments = {},
        .width       = w,
        .height      = h,
    };

    for (u8 i = 0; i < 3; ++i) {
        renderTargetProperties.attachments = {
            backendProxy.getFramebuffer(i), backendProxy.getDepthBuffer()
        };
        renderPassProperties.targets.push_back(renderTargetProperties);
    }

    m_renderPass = resourcePools.createRenderPass(renderPassProperties);
    m_shader->createPipeline(m_renderPass);
    LOG_TRACE("WorldRenderView initialized");
}

struct GeometryRenderData {
    Geometry* geometry;
    Material* material;
    Mat4f modelMatrix;
    float cameraDistance;
};

void WorldRenderView::render(
  RendererBackendProxy& backendProxy, const RenderPacket& packet,
  const RenderProperties& properties, [[maybe_unused]] float deltaTime
) {
    m_renderPass->run(
      *backendProxy.getCommandBuffer(), backendProxy.getImageIndex(),
      [&]() {
          glm::vec4 ambientColor(0.3f, 0.3f, 0.3f, 1.0f);

          const auto cameraPosition = m_camera->getPosition();

          m_shader->use();
          m_shader->setGlobalUniforms([&](Shader::UniformProxy& proxy) {
              proxy.set("view", m_camera->getViewMatrix());
              proxy.set("projection", m_camera->getProjectionMatrix());
              proxy.set("viewPosition", cameraPosition);
              proxy.set("ambientColor", ambientColor);
              proxy.set("renderMode", static_cast<int>(properties.renderMode));
          });

          std::vector<GeometryRenderData> geometries;
          std::vector<GeometryRenderData> transparentGeometries;
          geometries.reserve(256);
          transparentGeometries.reserve(128);

          for (auto& model : packet.models) {
              for (const auto& geometry : model->geometries) {
                  // TODO: shouldn't the material be bound to model instead of
                  // geometry?
                  const auto& modelMatrix = model->getModelMatrix();
                  auto material           = geometry->getProperties().material;

                  if (material->isTransparent()) {
                      auto center = modelMatrix * geometry->getExtent().center;
                      auto cameraDistance = glm::distance2(cameraPosition, center);
                      transparentGeometries.emplace_back(
                        geometry, material, modelMatrix, cameraDistance
                      );
                  } else {
                      geometries.emplace_back(geometry, material, modelMatrix);
                  }
              }
          }

          std::sort(
            transparentGeometries.begin(), transparentGeometries.end(),
            [](auto& lhs, auto& rhs) -> bool {
                return lhs.cameraDistance < rhs.cameraDistance;
            }
          );
          std::move(
            transparentGeometries.begin(), transparentGeometries.end(),
            std::back_inserter(geometries)
          );
          transparentGeometries.clear();

          for (auto& [geometry, material, model, _] : geometries) {
              m_shader->setLocalUniforms([&](Shader::UniformProxy& proxy) {
                  proxy.set("model", model);
              });

              material->applyUniforms(properties.frameNumber);
              backendProxy.drawGeometry(*geometry);
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
    m_renderPass->setArea(glm::vec4{ 0.0f, 0.0f, w, h });
}

}  // namespace sl