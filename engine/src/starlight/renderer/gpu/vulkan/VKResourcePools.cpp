#include "VKResourcePools.h"

#include "VKRendererBackend.h"

namespace sl::vk {

VKResourcePools::VKResourcePools(
  VKContext& context, VKDevice& device, VKBuffer& vertexBuffer,
  VKBuffer& indexBuffer, VKSwapchain& swapchain, VKRendererBackend* backend
) :
    m_context(context),
    m_device(device), m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer),
    m_swapchain(swapchain), backend(backend), m_textures("Texture", 1024),
    m_shaders("Shader", 1024), m_meshes("Mesh", vulkanMaxMeshCount),
    m_renderTargets("RenderTarget", 64), m_renderPasses("RenderPass", 64) {
    LOG_TRACE("VKResourcePools created");
}

VKMesh* VKResourcePools::createMesh(
  const Mesh::Properties& props, std::span<const Vertex3> vertices,
  std::span<const u32> indices, const Extent3& extent
) {
    Mesh::Data data(
      sizeof(Vertex3), vertices.size(), vertices.data(), indices, extent
    );
    return m_meshes.create(
      &m_device, &m_context, m_vertexBuffer, m_indexBuffer, props, data
    );
}

VKMesh* VKResourcePools::createMesh(
  const Mesh::Properties& props, std::span<const Vertex2> vertices,
  std::span<const u32> indices, const Extent2& extent
) {
    Mesh::Data data(
      sizeof(Vertex2), vertices.size(), vertices.data(), indices, Extent3{ extent }
    );
    return m_meshes.create(
      &m_device, &m_context, m_vertexBuffer, m_indexBuffer, props, data
    );
}

void VKResourcePools::destroyMesh(Mesh& mesh) { m_meshes.destroy(mesh.getId()); }

VKTexture* VKResourcePools::createTexture(
  const Texture::Properties& props, const std::span<u8> pixels
) {
    return m_textures.create(&m_context, &m_device, props, pixels);
}

void VKResourcePools::destroyTexture(Texture& texture) {
    m_textures.destroy(texture.getId());
}

VKShader* VKResourcePools::createShader(const Shader::Properties& props) {
    return m_shaders.create(&m_device, &m_context, *backend->getProxy(), props);
}

void VKResourcePools::destroyShader(Shader& shader) {
    m_shaders.destroy(shader.getId());
}

VKRenderTarget* VKResourcePools::createRenderTarget(
  const RenderTarget::Properties& props, RenderPass* renderPass
) {
    return m_renderTargets.create(
      m_context, m_device, static_cast<VKRenderPass*>(renderPass), props
    );
}
void VKResourcePools::destroyRenderTarget(RenderTarget& renderTarget) {
    m_renderTargets.destroy(renderTarget.getId());
}

VKRenderPass* VKResourcePools::createRenderPass(const RenderPass::Properties& props
) {
    return m_renderPasses.create(&m_context, &m_device, m_swapchain, props);
}

void VKResourcePools::destroyRenderPass(RenderPass& renderPass) {
    m_renderPasses.destroy(renderPass.getId());
}

}  // namespace sl::vk
