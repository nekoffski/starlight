#pragma once

#include <functional>

#include "RenderView.h"

#include "starlight/renderer/gpu/RenderPass.h"
#include "starlight/renderer/gpu/Shader.h"

namespace sl {

class WorldRenderView : public RenderView {
public:
    explicit WorldRenderView(Camera* camera, Shader* shader);

    void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) override;

    void render(
      RendererBackendProxy& backendProxy, const RenderPacket& packet,
      const RenderProperties& properties, float deltaTime
    ) override;

    void onViewportResize(RendererBackendProxy& backendProxy, Vec2u32 viewportSize)
      override;

private:
    Shader* m_shader;
};

}  // namespace sl