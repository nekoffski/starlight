#pragma once

#include <span>

#include "starlight/core/math/Vertex3.h"
#include "starlight/renderer/fwd.h"

namespace sl {

class GPUMemoryProxy {
   public:
    explicit GPUMemoryProxy(RendererBackend& rendererBackend);

    void acquireMaterialResources(Material& material);
    void releaseMaterialResources(Material& material);

    void acquireGeometryResources(
        Geometry& geometry, const std::span<Vertex3> vertices, std::span<uint32_t> indices
    );
    void releaseGeometryResources(Geometry& geometry);

   private:
    RendererBackend& m_rendererBackend;
};

}  // namespace sl