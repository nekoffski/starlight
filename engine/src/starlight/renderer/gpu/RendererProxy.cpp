#include "RendererProxy.h"

#include "RendererBackend.h"
#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/Material.h"

namespace sl {

RendererProxy::RendererProxy(RendererBackend& rendererBackend) :
    m_rendererBackend(rendererBackend) {}

u32 RendererProxy::getRenderPassId(const std::string& renderPass) const {
    return m_rendererBackend.getRenderPassId(renderPass);
}

std::unique_ptr<Shader::Impl> RendererProxy::createShaderImpl(Shader& shader) const {
    return m_rendererBackend.createShaderImpl(shader);
}

void RendererProxy::acquireGeometryResources(
  Geometry& geometry, std::span<Vertex3> vertices, std::span<uint32_t> indices
) {
    m_rendererBackend.acquireGeometryResources(
      geometry, sizeof(Vertex3), vertices.size(), vertices.data(), indices
    );
}

void RendererProxy::acquireGeometryResources(
  Geometry& geometry, std::span<Vertex2> vertices, std::span<uint32_t> indices
) {
    m_rendererBackend.acquireGeometryResources(
      geometry, sizeof(Vertex2), vertices.size(), vertices.data(), indices
    );
}

void RendererProxy::releaseGeometryResources(Geometry& geometry) {
    m_rendererBackend.releaseGeometryResources(geometry);
}

void RendererProxy::destroyTexture(Texture* texture) {
    m_rendererBackend.destroyTexture(texture);
}

Texture* RendererProxy::createTexture(
  const Texture::Properties& props, const void* pixels
) {
    return m_rendererBackend.createTexture(props, pixels);
}

}  // namespace sl
