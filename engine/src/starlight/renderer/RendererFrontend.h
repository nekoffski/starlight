#pragma once

#include <vector>
#include <functional>
#include <span>

#include "RenderPacket.h"
#include "RenderMode.h"
#include "FrameStatistics.h"
#include "Model.h"

#include "camera/Camera.h"
#include "views/RenderView.h"
#include "gpu/RendererBackend.h"
#include "gpu/Vendor.h"

namespace sl {

class RendererFrontend {
public:
    explicit RendererFrontend(Window& window, const Config& config);

    void init(std::span<RenderView*> renderViews);

    FrameStatistics getFrameStatistics();

    void renderFrame(float deltaTime, const RenderPacket& packet);

    void setRenderMode(RenderMode mode);

    ResourcePools* getResourcePools();

    void onViewportResize(u32 w, u32 h);

private:
    vk::VKRendererBackend m_backend;

    std::vector<RenderView*> m_renderViews;

    RenderMode m_renderMode;
    u16 m_framesSinceResize;
    bool m_resizing;

    FrameStatistics m_frameStatistics;

    u32 m_viewportWidth;
    u32 m_viewportHeight;
};

}  // namespace sl
