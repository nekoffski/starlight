#pragma once

#include <functional>

#include "RenderView.hh"

#include "starlight/renderer/gpu/RenderPass.hh"
#include "starlight/renderer/gpu/Shader.hh"

namespace sl {

class WorldRenderView : public RenderView {
public:
    explicit WorldRenderView(Shader* shader);

    void init(RendererBackend& renderer, const InitProperties& initProperties)
      override;

    void render(
      RendererBackend& renderer, const RenderPacket& packet,
      const RenderProperties& properties, float deltaTime
    ) override;

    void onViewportResize(RendererBackend& renderer, Vec2<u32> viewportSize)
      override;

private:
    Shader* m_shader;
};

}  // namespace sl