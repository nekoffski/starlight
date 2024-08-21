#pragma once

#include "starlight/core/memory/Memory.hpp"

#include "RenderView.h"

#include "starlight/renderer/gpu/RenderPass.h"
#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/Skybox.h"

namespace sl {

class SkyboxRenderView : public RenderView {
public:
    using UICallback = std::function<void(float)>;

    explicit SkyboxRenderView(Camera* camera, Skybox* skybox);

    void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) override;

    void render(
      RendererBackendProxy& backendProxy, const RenderPacket& packet,
      const RenderProperties& properties, float deltaTime
    ) override;

    void onViewportResize(RendererBackendProxy& backendProxy, Vec2<u32> viewportSize)
      override;

private:
    Skybox* m_skybox;
};

}  // namespace sl