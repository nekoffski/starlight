#pragma once

#include <vector>
#include <functional>
#include <span>

#include "starlight/core/Context.h"

#include "RenderPacket.h"
#include "RenderMode.h"
#include "FrameStatistics.h"
#include "Model.h"
#include "Skybox.h"

#include "camera/Camera.h"
#include "views/RenderView.h"
#include "gpu/RendererBackend.h"
#include "gpu/Vendor.h"
#include "gpu/Shader.h"
#include "gpu/Texture.h"

namespace sl {

class RendererFrontend : public NonCopyable {
public:
    explicit RendererFrontend(Context& context);

    void init(std::span<RenderView*> renderViews);

    FrameStatistics getFrameStatistics();
    RendererBackend& getRendererBackend();

    void renderFrame(float deltaTime, const RenderPacket& packet);
    void setRenderMode(RenderMode mode);

    ResourcePools* getResourcePools();
    void onViewportResize(Vec2<u32> viewportSize);

private:
    RendererBackendVendor m_backend;

    std::vector<RenderView*> m_renderViews;

    RenderMode m_renderMode;
    u16 m_framesSinceResize;
    bool m_resizing;

    FrameStatistics m_frameStatistics;
    Vec2<u32> m_viewportSize;

    // manager singletons, we want to expicitly specify order of initialization
    ShaderManager m_shaderManager;
    TextureManager m_textureManager;
    SkyboxManager m_skyboxManager;
};

}  // namespace sl
