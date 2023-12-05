#include "Renderer.h"

namespace sl {

Renderer::Renderer(Window& window, const Config& config, Camera* camera) :
    m_backend(window, config), m_frontend(&m_backend, camera) {}

Renderer::~Renderer() { LOG_TRACE("Renderer destroyed"); }

void Renderer::setCoreShaders(Shader* uiShader, Shader* materialShader) {
    m_frontend.setCoreShaders(uiShader, materialShader);
}

void Renderer::setRenderMode(RenderMode mode) { m_frontend.setRenderMode(mode); }

FrameStatistics Renderer::renderFrame(float deltaTime) {
    return m_frontend.renderFrame(deltaTime);
}

void Renderer::addUIPass(std::function<void()>&& callback) {
    m_frontend.addUIPass(std::move(callback));
}

void Renderer::addMainPass(RenderPacket& renderPacket) {
    m_frontend.addMainPass(renderPacket);
}

void Renderer::resizeViewport(u32 width, u32 height) {
    // TODO: handle changing viewport as different event, viewport.size !=
    // window.size (GUI)
    LOG_TRACE("Window resized, handling viewport change on renderer backend");
    m_frontend.onViewportResize(width, height);
}

void Renderer::setCamera(Camera* camera) { m_frontend.setCamera(camera); }

ResourcePools* Renderer::getResourcePools() { return m_backend.getResourcePools(); }

}  // namespace sl
