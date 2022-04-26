#pragma once

#include <kc/event/EventListener.h>

#include <unordered_map>

#include "ViewFrustum.h"
#include "sl/cam/Camera.h"
#include "sl/geom/Mesh.h"
#include "sl/gfx/RenderApi.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/VertexArray.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

namespace sl::gfx {

struct RendererSettings {
    bool enableDepthMask     = true;
    bool enableBlending      = false;
    unsigned int polygonMode = STARL_FILL;
    unsigned int cullFace    = STARL_BACK;
};

class Renderer : public kc::event::EventListener {
   public:
    explicit Renderer(gfx::RenderApi* renderApi, const Viewport& viewport);

    void renderVertexArray(gfx::VertexArray&);

    void renderLine();
    void clearBuffers(unsigned buffers);

    void setSettings(const RendererSettings& settings);
    void setTemporarySettings(const RendererSettings& settings);
    void restoreSettings();
    const RendererSettings& getSettings() const;

    void setTemporaryViewport(const Viewport& viewport);
    void restoreViewport();

   private:
    void handleEvents(const kc::event::EventProvider& eventProvider) override;

    void applySettings(const RendererSettings& settings);

    gfx::RenderApi* m_renderApi;

    Viewport m_viewport;
    RendererSettings m_settings;
};
}  // namespace sl::gfx
