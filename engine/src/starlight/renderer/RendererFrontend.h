#pragma once

#include <functional>
#include <span>

#include "camera/Camera.h"
#include "RendererBackend.h"
#include "Shader.h"
#include "RenderPacket.h"
#include "RenderMode.h"
#include "FrameStatistics.h"
#include "views/RenderView.h"

#include "Mesh.h"
#include "gpu/Vendor.h"

namespace sl {

class RendererFrontend {
public:
    explicit RendererFrontend(Window& window, const Config& config, Camera* camera);
    virtual ~RendererFrontend();

    void init(std::span<RenderView*> renderViews);

    FrameStatistics renderFrame(float deltaTime, std::span<Mesh> meshes);

    void setRenderMode(RenderMode mode);

    ResourcePools* getResourcePools();

    void onViewportResize(u32 w, u32 h);

private:
    RendererBackendVendor m_backend;

    std::vector<RenderView*> m_renderViews;

    RenderMode m_renderMode;

    u64 m_frameNumber;
    u16 m_framesSinceResize;
    bool m_resizing;

    u32 m_viewportWidth;
    u32 m_viewportHeight;
};

}  // namespace sl
