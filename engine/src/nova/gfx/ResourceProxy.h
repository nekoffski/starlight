#pragma once

#include <span>

#include "nova/math/Vertex3.h"

#include "fwd.h"

namespace nova::gfx {

class ResourceProxy {
   public:
    ResourceProxy(RendererBackend& rendererBackend);

    void acquireMaterialResources(gfx::Material& material);
    void releaseMaterialResources(gfx::Material& material);

    void acquireGeometryResources(
        gfx::Geometry& geometry, const std::span<math::Vertex3> vertices,
        std::span<uint32_t> indices
    );
    void releaseGeometryResources(gfx::Geometry& geometry);

   private:
    RendererBackend& m_rendererBackend;
};

}  // namespace nova::gfx