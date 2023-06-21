#pragma once

// #include "starlight/event/Event.h"

#include "starlight/renderer/gpu/RendererBackend.h"
#include "starlight/renderer/camera/Camera.h"

#include "RenderPacket.h"

namespace sl {

class RendererFrontend {
   public:
    explicit RendererFrontend(RendererBackend* backend);
    virtual ~RendererFrontend();

    bool drawFrame(const RenderPacket& renderPacket, const Camera& camera, float deltaTime);

   private:
    RendererBackend* m_backend;

    Texture* m_activeTexture;
    Texture* m_texture1;
    Texture* m_texture2;
    Material* m_material;
};

}  // namespace sl
