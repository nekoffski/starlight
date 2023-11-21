#include "VKResourcePools.h"

#include "VKRendererBackend.h"

namespace sl::vk {

VKResourcePools::VKResourcePools(
  VKContext& context, VKDevice& device, VKBuffer& vertexBuffer,
  VKBuffer& indexBuffer, VKRendererContext& rendererContext,
  VKRendererBackend* backend
) :
    m_context(context),
    m_device(device), m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer),
    m_rendererContext(rendererContext), backend(backend),
    m_textures("Texture", 1024), m_textureMaps("TextureMap", 1024),
    m_shaders("Shader", 1024), m_geometries("Geometry", vulkanMaxGeometryCount) {}

Geometry* VKResourcePools::createGeometry(
  const Geometry::Properties& props, const std::span<Vertex3> vertices,
  const std::span<uint32_t> indices
) {
    Geometry::Data data(sizeof(Vertex3), vertices.size(), vertices.data(), indices);
    return m_geometries.create(
      &m_device, &m_context, m_vertexBuffer, m_indexBuffer, props, data
    );
}

Geometry* VKResourcePools::createGeometry(
  const Geometry::Properties& props, const std::span<Vertex2> vertices,
  const std::span<uint32_t> indices
) {
    Geometry::Data data(sizeof(Vertex2), vertices.size(), vertices.data(), indices);
    return m_geometries.create(
      &m_device, &m_context, m_vertexBuffer, m_indexBuffer, props, data
    );
}

void VKResourcePools::destroyGeometry(Geometry& geometry) {
    m_geometries.destroy(geometry.getId());
}

Texture* VKResourcePools::createTexture(
  const Texture::Properties& props, const std::span<u8> pixels
) {
    return m_textures.create(&m_context, &m_device, props, pixels);
}

void VKResourcePools::destroyTexture(Texture& texture) {
    m_textures.destroy(texture.getId());
}

TextureMap* VKResourcePools::createTextureMap(
  const TextureMap::Properties& props, Texture& texture
) {
    return m_textureMaps.create(
      m_context, m_device, props, m_textures.get(texture.getId())
    );
}

void VKResourcePools::destroyTextureMap(TextureMap& textureMap) {
    m_textureMaps.destroy(textureMap.getId());
}

Shader* VKResourcePools::createShader(const Shader::Properties& props) {
    return m_shaders.create(
      &m_device, &m_context,
      backend->getRenderPass(backend->getRenderPassId(props.renderPassName)),
      m_rendererContext, props
    );
}

void VKResourcePools::destroyShader(Shader& shader) {
    m_shaders.destroy(shader.getId());
}

}  // namespace sl::vk