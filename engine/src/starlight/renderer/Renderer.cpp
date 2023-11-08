#include "Renderer.h"

namespace sl {

Renderer::Renderer(Window& window, const Config& config) :
    m_backend(window, config), m_frontend(&m_backend), m_rendererProxy(m_backend) {}

Renderer::~Renderer() { LOG_TRACE("Renderer destroyed"); }

void Renderer::setCoreShaders(Shader* uiShader, Shader* materialShader) {
    m_frontend.setCoreShaders(uiShader, materialShader);
}

void Renderer::setRenderMode(RenderMode mode) { m_frontend.setRenderMode(mode); }

bool Renderer::renderFrame(float deltaTime) {
    return m_frontend.renderFrame(deltaTime);
}

void Renderer::addUIPass(std::function<void()>&& callback) {
    m_frontend.addUIPass(std::move(callback));
}

void Renderer::addMainPass(RenderPacket& renderPacket, const Camera& camera) {
    m_frontend.addMainPass(renderPacket, camera);
}

void Renderer::resizeViewport(uint32_t width, uint32_t height) {
    // TODO: handle changing viewport as different event, viewport.size !=
    // window.size (GUI)
    LOG_TRACE("Window resized, handling viewport change on renderer backend");
    m_backend.onViewportResize(width, height);
}

RendererProxy* Renderer::getProxy() { return &m_rendererProxy; }

}  // namespace sl
