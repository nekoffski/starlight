#pragma once

#include "RendererBacked.h"
#include "nova/event/Event.h"
#include "Camera.h"

namespace nova::gfx {

struct RenderPacket {};

class RendererFrontend : public event::EventObserver {
   public:
    explicit RendererFrontend(RendererBackend* backend);

    virtual ~RendererFrontend();

    bool drawFrame(const RenderPacket& renderPacket, const Camera& camera, float deltaTime);
    void onEvent(event::EventWrapper& eventWrapper) override;

   private:
    RendererBackend* m_backend;

    Texture* m_activeTexture;
    Texture* m_texture1;
    Texture* m_texture2;
};

}  // namespace nova::gfx
