#pragma once

#include <unordered_map>

#include "sl/graphics/camera/Camera.h"

#include "sl/graphics/GraphicsContext.h"
#include "sl/graphics/RenderApi.h"
#include "sl/graphics/Shader.h"
#include "sl/graphics/Texture.h"
#include "sl/graphics/buffer/VertexArray.h"

#include "sl/math/Matrix.hpp"
#include "sl/math/Utils.hpp"

#include "sl/geometry/Mesh.h"

#include "ViewFrustum.h"

namespace sl::graphics {

struct RendererSettings {
    bool enableDepthMask = true;
    unsigned int polygonMode = STARL_FILL;
};

class LowLevelRenderer {
public:
    explicit LowLevelRenderer(std::shared_ptr<graphics::GraphicsContext> graphicsContext,
        std::unique_ptr<graphics::RenderApi> renderApi, ViewFrustum::Viewport viewport);

    void renderVertexArray(std::shared_ptr<graphics::buffer::VertexArray>);

    void clearBuffers(unsigned buff) {
        m_graphicsContext->clearBuffers(buff);
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
        m_graphicsContext->setViewport(viewport.width, viewport.height);
    }

    void restoreViewport() {
        m_graphicsContext->setViewport(m_viewport.width, m_viewport.height);
    }

    void setViewport(const ViewFrustum::Viewport& viewport) {
        m_viewport = viewport;
        restoreViewport();
    }

    void swapBuffers() {
        m_graphicsContext->swapBuffers();
    }

private:
    void applySettings(const RendererSettings& settings) {
        m_renderApi->depthMask(settings.enableDepthMask);
        m_renderApi->setPolygonMode(settings.polygonMode);
    }

    std::shared_ptr<graphics::GraphicsContext> m_graphicsContext;
    std::unique_ptr<graphics::RenderApi> m_renderApi;
    ViewFrustum::Viewport m_viewport;
    RendererSettings m_settings;
};
}
