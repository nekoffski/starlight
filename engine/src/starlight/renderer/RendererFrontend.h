#pragma once

#include <functional>

#include "starlight/renderer/camera/Camera.h"
#include "starlight/renderer/RendererBackend.h"

#include "Shader.h"

#include "RenderPacket.h"
#include "RenderMode.h"
#include "FrameStatistics.h"

#include "gpu/Vendor.h"

namespace sl {

class RendererFrontend {
    struct RenderPass {
        u32 id;
        std::function<void()> callback;
    };

public:
    explicit RendererFrontend(Window& window, const Config& config, Camera* camera);
    virtual ~RendererFrontend();

    FrameStatistics renderFrame(float deltaTime);

    void addUIPass(std::function<void()>&& callback);
    void addMainPass(RenderPacket& renderPacket);

    void setCoreShaders(Shader* uiShader, Shader* materialShader);
    void setRenderMode(RenderMode mode);
    void setCamera(Camera* camera);

    ResourcePools* getResourcePools();

    void onViewportResize(u32 w, u32 h);

private:
    RendererBackendVendor m_backend;

    std::vector<RenderPass> m_renderPasses;

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
