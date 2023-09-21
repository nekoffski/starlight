#pragma once

#include "starlight/core/Config.h"
#include "starlight/core/window/Window.h"

#include "camera/Camera.h"

#include "gpu/Vendor.h"
#include "gpu/TextureLoader.h"
#include "gpu/GPUMemoryProxy.h"

#include "RendererFrontend.h"

namespace sl {

class Renderer {
  public:
    explicit Renderer(sl::Window& window, const sl::Config& config) :
        m_backend(window, config), m_frontend(&m_backend), m_gpuMemoryProxy(m_backend) {}

    bool drawFrame(const RenderPacket& renderPacket, const Camera& camera, float deltaTime) {
        return m_frontend.drawFrame(renderPacket, camera, deltaTime);
    }

    void resizeViewport(uint32_t width, uint32_t height) {
        // TODO: handle changing viewport as different event, viewport.size != window.size (GUI)
        LOG_TRACE("Window resized, handling viewport change on renderer backend");
        m_backend.onViewportResize(width, height);
    }

    TextureLoader* getTextureLoader() { return m_backend.getTextureLoader(); }
    GPUMemoryProxy* getGPUMemoryProxy() { return &m_gpuMemoryProxy; }

  private:
    RendererBackendVendor m_backend;
    RendererFrontend m_frontend;
    GPUMemoryProxy m_gpuMemoryProxy;
};

}  // namespace sl
