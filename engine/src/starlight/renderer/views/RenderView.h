#pragma once

#include "starlight/renderer/gpu/RendererBackendProxy.h"
#include "starlight/renderer/gpu/RenderPass.h"
#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/camera/Camera.h"
#include "starlight/renderer/gpu/ResourcePools.h"
#include "starlight/renderer/RenderPacket.h"
#include "starlight/renderer/RenderProperties.h"

namespace sl {

class RenderView {
public:
    struct InitProperties {
        Vec2<u32> viewportSize;
        bool hasNextView;
        bool hasPreviousView;
    };

    explicit RenderView(Camera* camera);
    virtual ~RenderView() = default;

    void setCamera(Camera* camera);

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
    Camera* m_camera;
};

}  // namespace sl
