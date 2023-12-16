#pragma once

#include <functional>

#include "RenderView.h"
#include "starlight/renderer/RenderPass.h"

namespace sl {

class UIRenderView : public RenderView {
public:
    using UICallback = std::function<void()>;

    explicit UIRenderView(Camera* camera, UICallback&& callback);

    void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      u32 viewportWidth, u32 viewportHeight
    ) override;
    void render(RendererBackendProxy& backendProxy, const RenderPacket& renderPacket)
      override;
    void onViewportResize(RendererBackendProxy& backendProxy, u32 w, u32 h) override;

private:
    UICallback m_uiCallback;
};

}  // namespace sl