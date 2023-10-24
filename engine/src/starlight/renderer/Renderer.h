#pragma once

#include "starlight/core/Config.h"
#include "starlight/core/window/Window.h"

#include "camera/Camera.h"

#include "gpu/RendererProxy.h"
#include "gpu/TextureLoader.h"
#include "gpu/Vendor.h"

#include "RendererFrontend.h"
#include "Shader.h"

namespace sl {

class Renderer {
public:
    explicit Renderer(sl::Window& window, const sl::Config& config) :
        m_backend(window, config), m_frontend(&m_backend),
        m_rendererProxy(m_backend) {}

    ~Renderer() { LOG_TRACE("Renderer destroyed"); }

    void setCoreShaders(Shader* uiShader, Shader* materialShader) {
        m_frontend.setCoreShaders(uiShader, materialShader);
    }

    bool drawFrame(
      RenderPacket& renderPacket, const Camera& camera, float deltaTime
    ) {
        return m_frontend.drawFrame(renderPacket, camera, deltaTime);
    }

    void setRenderMode(RenderMode mode) { m_frontend.setRenderMode(mode); }

    void resizeViewport(uint32_t width, uint32_t height) {
        // TODO: handle changing viewport as different event, viewport.size !=
        // window.size (GUI)
        LOG_TRACE("Window resized, handling viewport change on renderer backend");
        m_backend.onViewportResize(width, height);
    }

    TextureLoader* getTextureLoader() { return m_backend.getTextureLoader(); }
    RendererProxy* getProxy() { return &m_rendererProxy; }

private:
    RendererBackendVendor m_backend;
    RendererFrontend m_frontend;
    RendererProxy m_rendererProxy;
};

}  // namespace sl
