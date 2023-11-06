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
    explicit Renderer(Window& window, const Config& config);

    ~Renderer();

    void setCoreShaders(Shader* uiShader, Shader* materialShader);
    void setRenderMode(RenderMode mode);

    bool drawFrame(
      RenderPacket& renderPacket, const Camera& camera, float deltaTime
    );

    void resizeViewport(uint32_t width, uint32_t height);

    TextureLoader* getTextureLoader();
    RendererProxy* getProxy();

private:
    RendererBackendVendor m_backend;
    RendererFrontend m_frontend;
    RendererProxy m_rendererProxy;
};

}  // namespace sl
