#pragma once

#include <functional>

#include "starlight/renderer/camera/Camera.h"
#include "starlight/renderer/gpu/RendererBackend.h"

#include "Shader.h"

#include "RenderPacket.h"
#include "RenderMode.h"
#include "FrameStatistics.h"

namespace sl {

class RendererFrontend {
    struct RenderPass {
        u32 id;
        std::function<void()> callback;
    };

public:
    explicit RendererFrontend(RendererBackend* backend, Camera* camera);
    virtual ~RendererFrontend();

    FrameStatistics renderFrame(float deltaTime);

    void addUIPass(std::function<void()>&& callback);
    void addMainPass(RenderPacket& renderPacket);

    void setCoreShaders(Shader* uiShader, Shader* materialShader);
    void setRenderMode(RenderMode mode);
    void setCamera(Camera* camera);

    void onViewportResize(u32 w, u32 h);

private:
    std::vector<RenderPass> m_renderPasses;

    RendererBackend* m_backend;
    Camera* m_camera;

    Texture* m_activeTexture;
    Texture* m_texture1;
    Texture* m_texture2;
    Material* m_material;

    Shader* m_materialShader;
    Shader* m_uiShader;

    RenderMode m_renderMode;

    u64 m_frameNumber;
    u16 m_framesSinceResize;
    bool m_resizing;

    u32 m_viewportWidth;
    u32 m_viewportHeight;
};

}  // namespace sl
