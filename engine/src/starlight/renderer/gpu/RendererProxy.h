#pragma once

#include <span>

#include "starlight/core/Core.h"
#include "starlight/core/math/Vertex.h"

#include "starlight/renderer/Shader.h"
#include "starlight/renderer/Texture.h"
#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/fwd.h"

namespace sl {

class RendererProxy {
public:
    explicit RendererProxy(RendererBackend& rendererBackend);

    u32 getRenderPassId(const std::string& renderPass) const;

    std::unique_ptr<Shader::Impl> createShaderImpl(Shader& shader) const;

    Geometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex3> vertices,
      const std::span<uint32_t> indices
    );
    Geometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex2> vertices,
      const std::span<uint32_t> indices
    );
    void destroyGeometry(Geometry& geometry);

    Texture* createTexture(const Texture::Properties& props, const void* pixels);
    void destroyTexture(Texture& texture);

private:
    RendererBackend& m_rendererBackend;
};

}  // namespace sl