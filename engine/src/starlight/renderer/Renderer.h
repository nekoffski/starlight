#pragma once

#include "starlight/core/Config.h"
#include "starlight/core/window/Window.h"

#include "camera/Camera.h"

#include "gpu/Vendor.h"

#include "ResourcePools.h"
#include "RendererFrontend.h"
#include "Shader.h"

namespace sl {

class Renderer {
public:
    explicit Renderer(Window& window, const Config& config, Camera* camera);

    ~Renderer();

    void setCoreShaders(Shader* uiShader, Shader* materialShader);
    void setRenderMode(RenderMode mode);

    FrameStatistics renderFrame(float deltaTime);

    void addUIPass(std::function<void()>&& callback);
    void addMainPass(RenderPacket& renderPacket);

    void resizeViewport(uint32_t width, uint32_t height);

    void setCamera(Camera*);

    ResourcePools* getResourcePools();

private:
    RendererBackendVendor m_backend;
    RendererFrontend m_frontend;
};

}  // namespace sl
