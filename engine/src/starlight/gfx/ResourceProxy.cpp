#include "ResourceProxy.h"

#include "RendererBackend.h"
#include "Material.h"
#include "Geometry.h"

namespace sl::gfx {

ResourceProxy::ResourceProxy(RendererBackend& rendererBackend)
    : m_rendererBackend(rendererBackend) {}

void ResourceProxy::acquireMaterialResources(gfx::Material& material) {
    m_rendererBackend.acquireMaterialResources(material);
}

void ResourceProxy::releaseMaterialResources(gfx::Material& material) {
    m_rendererBackend.releaseMaterialResources(material);
}

void ResourceProxy::acquireGeometryResources(
    gfx::Geometry& geometry, std::span<math::Vertex3> vertices, std::span<uint32_t> indices
) {
    m_rendererBackend.acquireGeometryResources(geometry, vertices, indices);
}

void ResourceProxy::releaseGeometryResources(gfx::Geometry& geometry) {
    m_rendererBackend.releaseGeometryResources(geometry);
}

}  // namespace sl::gfx
