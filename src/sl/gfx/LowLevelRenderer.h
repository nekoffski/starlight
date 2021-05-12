#pragma once

#include <unordered_map>

#include "sl/gfx/camera/Camera.h"

#include "sl/gfx/GraphicsContext.h"
#include "sl/gfx/RenderApi.h"
#include "sl/gfx/Shader.h"
#include "sl/gfx/Texture.h"
#include "sl/gfx/buffer/VertexArray.h"

#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

#include "sl/geom/Mesh.h"

#include "ViewFrustum.h"

namespace sl::gfx {

struct RendererSettings {
    bool enableDepthMask = true;
    unsigned int polygonMode = STARL_FILL;
};

class LowLevelRenderer {
public:
    explicit LowLevelRenderer(std::shared_ptr<gfx::GraphicsContext> gfxContext,
        std::unique_ptr<gfx::RenderApi> renderApi, ViewFrustum::Viewport viewport);

    void renderVertexArray(std::shared_ptr<gfx::buffer::VertexArray>);

    void renderLine() {
        m_renderApi->drawArrays(STARL_LINES, 0, 2);
    }

    void clearBuffers(unsigned buff) {
        m_gfxContext->clearBuffers(buff);
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
        m_gfxContext->setViewport(viewport);
    }

    void restoreViewport() {
        m_gfxContext->setViewport(m_viewport);
    }

    void setViewport(const ViewFrustum::Viewport& viewport) {
        m_viewport = viewport;
        restoreViewport();
    }

    void swapBuffers() {
        m_gfxContext->swapBuffers();
    }

private:
    void applySettings(const RendererSettings& settings) {
        m_renderApi->depthMask(settings.enableDepthMask);
        m_renderApi->setPolygonMode(settings.polygonMode);
    }

    std::shared_ptr<gfx::GraphicsContext> m_gfxContext;
    std::unique_ptr<gfx::RenderApi> m_renderApi;
    ViewFrustum::Viewport m_viewport;
    RendererSettings m_settings;
};
}
