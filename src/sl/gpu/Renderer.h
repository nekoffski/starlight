#pragma once

#include <kc/event/EventListener.h>

#include <unordered_map>

#include "sl/gpu/Viewport.h"
#include "sl/cam/Camera.h"
#include "sl/geom/Mesh.h"

#include "RenderApi.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

namespace sl::gpu {

struct RendererSettings {
    bool enableDepthMask     = true;
    bool enableBlending      = false;
    unsigned int polygonMode = STARL_FILL;
    unsigned int cullFace    = STARL_BACK;
};

class Renderer : public kc::event::EventListener {
   public:
    explicit Renderer(gpu::RenderApi* renderApi, const Viewport& viewport);

    void renderVertexArray(gpu::VertexArray&);

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

    gpu::RenderApi* m_renderApi;

    Viewport m_viewport;
    RendererSettings m_settings;
};
}  // namespace sl::gpu
