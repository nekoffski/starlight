#pragma once

#include "starlight/renderer/ResourcePool.hpp"
#include "starlight/renderer/ResourcePools.h"

#include "VKTexture.h"
#include "VKShader.h"
#include "VKGeometry.h"
#include "VKContext.h"
#include "VKDevice.h"
#include "VKRendererContext.h"

#include "fwd.h"

namespace sl::vk {

class VKResourcePools : public ResourcePools {
public:
    explicit VKResourcePools(
      VKContext& context, VKDevice& device, VKBuffer& vertexBuffer,
      VKBuffer& indexBuffer, VKRendererContext& rendererContext,
      VKRendererBackend* backend
    );

    Geometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex3> vertices,
      const std::span<uint32_t> indices
    ) override;
    Geometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex2> vertices,
      const std::span<uint32_t> indices
    ) override;
    void destroyGeometry(Geometry& geometry) override;

    Texture* createTexture(
      const Texture::Properties& props, const std::span<u8> pixels
    ) override;
    void destroyTexture(Texture& texture) override;

    TextureMap* createTextureMap(
      const TextureMap::Properties& props, Texture& texture
    ) override;
    void destroyTextureMap(TextureMap& textureMap) override;

    Shader* createShader(const Shader::Properties& props) override;
    void destroyShader(Shader& shader) override;

private:
    VKContext& m_context;
    VKDevice& m_device;
    VKBuffer& m_vertexBuffer;
    VKBuffer& m_indexBuffer;
    VKRendererContext& m_rendererContext;

    VKRendererBackend* backend;  // TODO: temporary, remove when render pass creation
                                 // will be managed by this class, for now we need
                                 // this for render pass id

    ResourcePool<VKTexture> m_textures;
    ResourcePool<VKTextureMap> m_textureMaps;
    ResourcePool<VKShader> m_shaders;
    ResourcePool<VKGeometry> m_geometries;
};

}  // namespace sl::vk