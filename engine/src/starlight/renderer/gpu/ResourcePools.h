#pragma once

#include "starlight/core/math/Vertex.h"

#include "starlight/renderer/gpu/Shader.h"
#include "starlight/renderer/gpu/Texture.h"
#include "starlight/renderer/gpu/Mesh.h"
#include "starlight/renderer/gpu/RenderPass.h"
#include "starlight/renderer/gpu/RenderTarget.h"

#include "starlight/renderer/fwd.h"

namespace sl {

struct ResourcePools {
    virtual Mesh* createMesh(
      const Mesh::Properties& props, std::span<const Vertex3> vertices,
      std::span<const uint32_t> indices, const Extent3& extent
    ) = 0;
    virtual Mesh* createMesh(
      const Mesh::Properties& props, std::span<const Vertex2> vertices,
      std::span<const uint32_t> indices, const Extent2& extent
    )                                    = 0;
    virtual void destroyMesh(Mesh& mesh) = 0;

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