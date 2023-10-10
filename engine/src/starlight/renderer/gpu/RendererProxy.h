#pragma once

#include <span>

#include "starlight/core/Core.h"
#include "starlight/core/math/Vertex.h"

#include "starlight/renderer/Shader.h"
#include "starlight/renderer/fwd.h"

namespace sl {

class RendererProxy {
public:
    explicit RendererProxy(RendererBackend& rendererBackend);

    void acquireMaterialResources(Material& material);
    void releaseMaterialResources(Material& material);

    u32 getRenderPassId(const std::string& renderPass) const;

    std::unique_ptr<Shader::Impl> createShaderImpl(Shader& shader) const;

    void acquireGeometryResources(
      Geometry& geometry, const std::span<Vertex3> vertices,
      std::span<uint32_t> indices
    );

    void acquireGeometryResources(
      Geometry& geometry, const std::span<Vertex2> vertices,
      std::span<uint32_t> indices
    );

    void releaseGeometryResources(Geometry& geometry);

private:
    RendererBackend& m_rendererBackend;
};

}  // namespace sl