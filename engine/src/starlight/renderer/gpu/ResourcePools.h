#pragma once

#include "starlight/core/math/Vertex.h"

#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/gpu/Texture.h"
#include "starlight/renderer/gpu/Geometry.h"
#include "starlight/renderer/gpu/RenderPass.h"
#include "starlight/renderer/gpu/RenderTarget.h"

#include "starlight/renderer/fwd.h"

namespace sl {

struct ResourcePools {
    virtual Geometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex3> vertices,
      const std::span<uint32_t> indices, const Extent3& extent
    ) = 0;
    virtual Geometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex2> vertices,
      const std::span<uint32_t> indices, const Extent2& extent
    )                                                = 0;
    virtual void destroyGeometry(Geometry& geometry) = 0;

    virtual Texture* createTexture(
      const Texture::Properties& props, const std::span<u8> pixels
    )                                             = 0;
    virtual void destroyTexture(Texture& texture) = 0;

    virtual TextureMap* createTextureMap(
      const TextureMap::Properties& props, Texture& texture
    )                                                      = 0;
    virtual void destroyTextureMap(TextureMap& textureMap) = 0;

    virtual Shader* createShader(const Shader::Properties& props) = 0;
    virtual void destroyShader(Shader& shader)                    = 0;

    virtual RenderTarget* createRenderTarget(
      const RenderTarget::Properties& props, RenderPass* renderPass
    )                                                            = 0;
    virtual void destroyRenderTarget(RenderTarget& renderTarget) = 0;

    virtual RenderPass* createRenderPass(const RenderPass::Properties& props) = 0;
    virtual void destroyRenderPass(RenderPass& renderPass)                    = 0;
};

}  // namespace sl