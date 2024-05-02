#pragma once

#include <functional>

#include "starlight/core/memory/Memory.hpp"

#include "RenderView.h"

#include "starlight/renderer/gpu/RenderPass.h"
#include "starlight/renderer/gpu/UIRenderer.h"

namespace sl {

class UIRenderView : public RenderView {
public:
    struct Properties {
        std::vector<Font::Properties> fonts;
    };

    using UICallback = std::function<void()>;

    explicit UIRenderView(
      Camera* camera, const Properties& props, UICallback&& callback
    );

    void init(
      RendererBackendProxy& backendProxy, ResourcePools& resourcePools,
      const InitProperties& initProperties
    ) override;

    void render(
      RendererBackendProxy& backendProxy, const RenderPacket& packet,
      const RenderProperties& properties, float deltaTime
    ) override;

    void onViewportResize(RendererBackendProxy& backendProxy, Vec2<u32> viewportSize)
      override;

private:
    Properties m_props;
    UICallback m_uiCallback;
    UniqPtr<UIRenderer> m_uiRenderer;

    std::vector<Font*> m_fonts;
};

}  // namespace sl