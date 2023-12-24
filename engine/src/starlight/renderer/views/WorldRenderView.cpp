#include "WorldRenderView.h"

namespace sl {

WorldRenderView::WorldRenderView(Camera* camera, Shader* shader) :
    RenderView(camera), m_shader(shader) {}

void WorldRenderView::init(
  RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
  const InitProperties& initProperties
) {
    auto backgroundColor = (1.0f / 255.0f) * glm::vec4{ 11, 16, 47, 255 };

    const auto w = initProperties.viewportWidth;
    const auto h = initProperties.viewportHeight;

    RenderPass::Properties renderPassProperties{
        .area       = glm::vec4{0.0f, 0.0f, w, h},
        .clearColor = backgroundColor,
        .clearFlags =
          (RenderPass::clearColorBuffer | RenderPass::clearDepthBuffer
           | RenderPass::clearStencilBuffer),
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
}

struct GeometryRenderData {
    Geometry* geometry;
    Material* material;
    Mat4f modelMatrix;
    float cameraDistance;
};

void WorldRenderView::render(
  RendererBackendProxy& backendProxy, const RenderPacket& renderPacket
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
              proxy.set("renderMode", static_cast<int>(renderPacket.renderMode));
          });

          std::vector<GeometryRenderData> geometries;
          std::vector<GeometryRenderData> transparentGeometries;
          geometries.reserve(256);
          transparentGeometries.reserve(128);

          for (auto& mesh : renderPacket.meshes) {
              const auto model = mesh.transform.getWorld();
              for (const auto& geometry : mesh.geometries) {
                  // TODO: shouldn't the material be bound to mesh instead of
                  // geometry?
                  auto material = geometry->getProperties().material;

                  if (material->isTransparent()) {
                      auto center         = model * geometry->getExtent().center;
                      auto cameraDistance = glm::distance2(cameraPosition, center);
                      transparentGeometries.emplace_back(
                        geometry, material, model, cameraDistance
                      );
                  } else {
                      geometries.emplace_back(geometry, material, model);
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

          for (auto& [geometry, material, model, _] : geometries) {
              m_shader->setLocalUniforms([&](Shader::UniformProxy& proxy) {
                  proxy.set("model", model);
              });

              material->applyUniforms(renderPacket.frameNumber);
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