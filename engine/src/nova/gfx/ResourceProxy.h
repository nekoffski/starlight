#pragma once

#include "fwd.h"

namespace nova::gfx {

class ResourceProxy {
   public:
    ResourceProxy(RendererBackend& rendererBackend);

    void acquireMaterialResources(gfx::Material& material);
    void releaseMaterialResources(gfx::Material& material);

   private:
    RendererBackend& m_rendererBackend;
};

}  // namespace nova::gfx