#pragma once

#include <functional>

#include "starlight/core/memory/Memory.hpp"

#include "RenderView.h"

#include "starlight/renderer/RenderPass.h"
#include "starlight/renderer/ui/UIRenderer.h"

namespace sl {

class UIRenderView : public RenderView {
public:
    using UICallback = std::function<void(float)>;

    explicit UIRenderView(Camera* camera, UICallback&& callback);

    void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) override;

    void render(RendererBackendProxy& backendProxy, const RenderPacket& renderPacket)
      override;

    void onViewportResize(RendererBackendProxy& backendProxy, u32 w, u32 h) override;

private:
    UICallback m_uiCallback;
    UniqPtr<UIRenderer> m_uiRenderer;
};

}  // namespace sl