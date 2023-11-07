#pragma once

#include <functional>

#include "starlight/renderer/camera/Camera.h"
#include "starlight/renderer/gpu/RendererBackend.h"

#include "Shader.h"

#include "RenderPacket.h"
#include "RenderMode.h"

namespace sl {

class RendererFrontend {
    struct RenderPass {
        u32 id;
        std::function<void()> callback;
    };

public:
    explicit RendererFrontend(RendererBackend* backend);
    virtual ~RendererFrontend();

    bool renderFrame(float deltaTime);

    void addUIPass(std::function<void()>&& callback);
    void addMainPass(RenderPacket& renderPacket, const Camera& camera);

    void setCoreShaders(Shader* uiShader, Shader* materialShader);
    void setRenderMode(RenderMode mode);

private:
    std::vector<RenderPass> m_renderPasses;

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
