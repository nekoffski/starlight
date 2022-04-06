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
    bool enableDepthMask = true;
    bool enableBlending = false;
    unsigned int polygonMode = STARL_FILL;
    unsigned int cullFace = STARL_BACK;
};

class Renderer : public kc::event::EventListener {
   public:
    explicit Renderer(gfx::RenderApi* renderApi, const ViewFrustum::Viewport& viewport);

    void renderVertexArray(gfx::VertexArray&);

    void renderLine() { m_renderApi->drawArrays(STARL_LINES, 0, 2); }

    void clearBuffers(unsigned buffers) { m_renderApi->clearBuffers(buffers); }

    void setSettings(const RendererSettings& settings) {
        m_settings = settings;
        applySettings(settings);
    }

    void setTemporarySettings(const RendererSettings& settings) { applySettings(settings); }

    void restoreSettings() { applySettings(m_settings); }

    const RendererSettings& getSettings() { return m_settings; }

    void setTemporaryViewport(const ViewFrustum::Viewport& viewport) {
        m_renderApi->setViewport(viewport);
    }

    void setViewport(const ViewFrustum::Viewport& viewport) {
        m_viewport = viewport;
        LOG_TRACE("Setting renderer viewport: {}", toString(m_viewport));
        m_renderApi->setViewport(m_viewport);
    }

    void restoreViewport() { m_renderApi->setViewport(m_viewport); }

   private:
    void handleEvents(const kc::event::EventProvider& eventProvider) override {
        auto events = eventProvider.getByCategories<sl::event::CoreCategory>();

        for (auto& event : events) {
            if (event->is<sl::event::ChangeViewportEvent>()) {
                // m_viewport = event->asView<sl::event::ChangeViewportEvent>()->viewport;
                LOG_TRACE("Setting renderer viewport: {}", toString(m_viewport));
                // m_renderApi->setViewport(m_viewport);
            }
        }
    }

    void applySettings(const RendererSettings& settings) {
        m_renderApi->depthMask(settings.enableDepthMask);
        m_renderApi->setPolygonMode(settings.polygonMode);

        if (settings.enableBlending)
            m_renderApi->enable(STARL_BLEND);
        else
            m_renderApi->disable(STARL_BLEND);
    }

    gfx::RenderApi* m_renderApi;

    ViewFrustum::Viewport m_viewport;
    RendererSettings m_settings;
};
}  // namespace sl::gfx
