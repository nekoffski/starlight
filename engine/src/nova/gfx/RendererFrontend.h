#pragma once

#include "RendererBacked.h"

namespace nova::gfx {

struct RenderPacket {
    float deltaTime;
};

class RendererFrontend {
   public:
    explicit RendererFrontend(RendererBackend* backend) : m_backend(backend) {}

    virtual ~RendererFrontend() {}

    bool drawFrame(const RenderPacket& renderPacket) {
        if (m_backend->beginFrame()) {
            return m_backend->endFrame();
        }

        return true;
    }

   private:
    RendererBackend* m_backend;
};

}  // namespace nova::gfx
