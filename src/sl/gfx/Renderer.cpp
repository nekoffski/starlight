#include "Renderer.h"

#include <kc/core/Log.h>

#include "sl/event/EventManager.h"

namespace sl::gfx {

Renderer::Renderer(gfx::RenderApi* renderApi, const Viewport& viewport)
    : EventListener("Renderer"), m_renderApi(renderApi), m_viewport(viewport) {
    sl::event::EventManager::get().registerListener(this);

    m_renderApi->enable(STARL_DEPTH_TEST);
    // m_renderApi->enable(STARL_BLEND);
    m_renderApi->setCullFace(STARL_BACK);
    m_renderApi->setDepthFunc(STARL_LEQUAL);
    m_renderApi->setBlendFunc(STARL_SRC_ALPHA, STARL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::renderVertexArray(gfx::VertexArray& vertexArray) {
    // to optimize! -> BRANCHING!!!
    if (auto indices = vertexArray.getIndicesCount(); indices != 0u)
        m_renderApi->drawElements(STARL_TRIANGLES, indices, STARL_UNSIGNED_INT);
    else
        m_renderApi->drawArrays(STARL_TRIANGLES, 0, vertexArray.getVerticesCount());
}

void Renderer::renderLine() { m_renderApi->drawArrays(STARL_LINES, 0, 2); }

void Renderer::clearBuffers(unsigned buffers) { m_renderApi->clearBuffers(buffers); }

void Renderer::setSettings(const RendererSettings& settings) {
    m_settings = settings;
    applySettings(settings);
}

void Renderer::setTemporarySettings(const RendererSettings& settings) { applySettings(settings); }

void Renderer::restoreSettings() { applySettings(m_settings); }

const RendererSettings& Renderer::getSettings() const { return m_settings; }

void Renderer::setTemporaryViewport(const Viewport& viewport) {
    m_renderApi->setViewport(viewport);
}

void Renderer::restoreViewport() { m_renderApi->setViewport(m_viewport); }

void Renderer::handleEvents(const kc::event::EventProvider& eventProvider) {
    auto events = eventProvider.getByCategories<sl::event::CoreCategory>();

    for (auto& event : events) {
        if (event->is<sl::event::ChangeViewportEvent>()) {
            m_viewport = event->asView<sl::event::ChangeViewportEvent>()->viewport;
            LOG_TRACE("Setting renderer viewport: {}", toString(m_viewport));
            m_renderApi->setViewport(m_viewport);
        }
    }
}

void Renderer::applySettings(const RendererSettings& settings) {
    m_renderApi->depthMask(settings.enableDepthMask);
    m_renderApi->setPolygonMode(settings.polygonMode);

    if (settings.enableBlending)
        m_renderApi->enable(STARL_BLEND);
    else
        m_renderApi->disable(STARL_BLEND);
}

}  // namespace sl::gfx
