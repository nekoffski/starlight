#pragma once

#include "starlight/renderer/RendererBackendProxy.h"
#include "starlight/renderer/RenderPass.h"
#include "starlight/renderer/Shader.h"
#include "starlight/renderer/camera/Camera.h"
#include "starlight/renderer/ResourcePools.h"
#include "starlight/renderer/RenderPacket.h"

namespace sl {

class RenderView {
public:
    struct InitProperties {
        u32 viewportWidth;
        u32 viewportHeight;
        bool hasNextView;
        bool hasPreviousView;
    };

    explicit RenderView(Camera* camera);

    void setCamera(Camera* camera);

    virtual void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) = 0;
    virtual void render(
      RendererBackendProxy& backendProxy, const RenderPacket& renderPacket
    ) = 0;
    virtual void onViewportResize(
      RendererBackendProxy& backendProxy, u32 w, u32 h
    ) = 0;

protected:
    RenderPass* m_renderPass;
    Camera* m_camera;
};

}  // namespace sl
