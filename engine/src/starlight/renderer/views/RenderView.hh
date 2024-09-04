#pragma once

#include "starlight/renderer/gpu/RendererBackendProxy.hh"
#include "starlight/renderer/gpu/RenderPass.hh"
#include "starlight/renderer/gpu/Shader.hh"
#include "starlight/renderer/camera/Camera.hh"
#include "starlight/renderer/gpu/ResourcePools.hh"
#include "starlight/renderer/RenderPacket.hh"
#include "starlight/renderer/RenderProperties.hh"

namespace sl {

class RenderView {
public:
    struct InitProperties {
        Vec2<u32> viewportSize;
        bool hasNextView;
        bool hasPreviousView;
    };

    explicit RenderView();
    virtual ~RenderView() = default;

    virtual void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) = 0;
    virtual void render(
      RendererBackendProxy& backendProxy, const RenderPacket& packet,
      const RenderProperties& properties, float deltaTime
    ) = 0;
    virtual void onViewportResize(
      RendererBackendProxy& backendProxy, Vec2<u32> viewportSize
    ) = 0;

protected:
    RenderPass* m_renderPass;
};

}  // namespace sl
