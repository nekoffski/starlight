#pragma once

// #include "starlight/event/Event.h"

#include "starlight/renderer/camera/Camera.h"
#include "starlight/renderer/gpu/RendererBackend.h"

#include "Shader.h"

#include "RenderPacket.h"
#include "RenderMode.h"

namespace sl {

class RendererFrontend {
public:
    explicit RendererFrontend(RendererBackend* backend);
    virtual ~RendererFrontend();

    bool drawFrame(
      RenderPacket& renderPacket, const Camera& camera, float deltaTime
    );

    void setCoreShaders(Shader* uiShader, Shader* materialShader);
    void setRenderMode(RenderMode mode);

private:
    RendererBackend* m_backend;

    Texture* m_activeTexture;
    Texture* m_texture1;
    Texture* m_texture2;
    Material* m_material;

    Shader* m_materialShader;
    Shader* m_uiShader;

    RenderMode m_renderMode;

    u64 m_frameNumber;
};

}  // namespace sl
