#include "RendererProxy.h"

#include "RendererBackend.h"
#include "starlight/renderer/Geometry.h"
#include "starlight/renderer/Material.h"

namespace sl {

RendererProxy::RendererProxy(RendererBackend& rendererBackend) :
    m_rendererBackend(rendererBackend) {}

void RendererProxy::acquireMaterialResources(Material& material) {
    m_rendererBackend.acquireMaterialResources(material);
}

void RendererProxy::releaseMaterialResources(Material& material) {
    m_rendererBackend.releaseMaterialResources(material);
}

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

}  // namespace sl
