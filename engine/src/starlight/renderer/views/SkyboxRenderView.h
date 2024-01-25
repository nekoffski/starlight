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

    explicit SkyboxRenderView(Camera* camera, Shader* skyboxShader, Skybox* skybox);

    void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) override;

    void render(
      RendererBackendProxy& backendProxy, const RenderPacket& packet,
      const RenderProperties& properties, float deltaTime
    ) override;

    void onViewportResize(RendererBackendProxy& backendProxy, u32 w, u32 h) override;

private:
    Shader* m_shader;
    Skybox* m_skybox;
};

}  // namespace sl