#pragma once

#include <functional>

#include "RenderView.h"

#include "starlight/renderer/RenderPass.h"
#include "starlight/renderer/Shader.h"

namespace sl {

class WorldRenderView : public RenderView {
public:
    explicit WorldRenderView(Camera* camera, Shader* shader);

    void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) override;

    void render(RendererBackendProxy& backendProxy, const RenderPacket& renderPacket)
      override;
    void onViewportResize(RendererBackendProxy& backendProxy, u32 w, u32 h) override;

private:
    Shader* m_shader;
};

}  // namespace sl