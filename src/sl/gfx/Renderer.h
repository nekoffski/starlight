#pragma once

#include <unordered_map>

#include "sl/gfx/camera/Camera.h"

#include "sl/gfx/RenderApi.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/VertexArray.h"

#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

#include "sl/geom/Mesh.h"

#include "ViewFrustum.h"

namespace sl::gfx {

struct RendererSettings {
    bool enableDepthMask = true;
    bool enableBlending = false;
    unsigned int polygonMode = STARL_FILL;
    unsigned int cullFace = STARL_BACK;
};

class Renderer {
public:
    explicit Renderer(gfx::RenderApi* renderApi, const ViewFrustum::Viewport& viewport);

    void renderVertexArray(gfx::VertexArray&);

    void renderLine() {
        m_renderApi->drawArrays(STARL_LINES, 0, 2);
    }

    void clearBuffers(unsigned buffers) {
        m_renderApi->clearBuffers(buffers);
    }

    void setSettings(const RendererSettings& settings) {
        m_settings = settings;
        applySettings(settings);
    }

    void setTemporarySettings(const RendererSettings& settings) {
        applySettings(settings);
    }

    void restoreSettings() {
        applySettings(m_settings);
    }

    const RendererSettings& getSettings() {
        return m_settings;
    }

    void setTemporaryViewport(const ViewFrustum::Viewport& viewport) {
        m_renderApi->setViewport(viewport);
    }

    void restoreViewport() {
        m_renderApi->setViewport(m_viewport);
    }

    ViewFrustum::Viewport getViewport() const {
        return m_viewport;
    }

    void setViewport(const ViewFrustum::Viewport& viewport) {
        m_viewport = viewport;
        restoreViewport();
    }

private:
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
}
