#pragma once

#include "starlight/core/memory/Memory.hh"

#include "RenderView.hh"

#include "starlight/renderer/gpu/RenderPass.hh"
#include "starlight/renderer/gpu/Shader.hh"
#include "starlight/renderer/Skybox.hh"

namespace sl {

class SkyboxRenderView : public RenderView {
public:
    using UICallback = std::function<void(float)>;

    explicit SkyboxRenderView(Skybox* skybox);

    void init(RendererBackend& backend, const InitProperties& initProperties)
      override;

    void render(
      RendererBackend& backend, const RenderPacket& packet,
      const RenderProperties& properties, float deltaTime
    ) override;

    void onViewportResize(RendererBackend& backend, Vec2<u32> viewportSize) override;

private:
    Skybox* m_skybox;
};

}  // namespace sl