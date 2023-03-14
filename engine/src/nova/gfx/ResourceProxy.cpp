#include "ResourceProxy.h"

#include "RendererBackend.h"
#include "Material.h"

namespace nova::gfx {

ResourceProxy::ResourceProxy(RendererBackend& rendererBackend)
    : m_rendererBackend(rendererBackend) {}

void ResourceProxy::acquireMaterialResources(gfx::Material& material) {
    m_rendererBackend.acquireMaterialResources(material);
}

void ResourceProxy::releaseMaterialResources(gfx::Material& material) {
    m_rendererBackend.releaseMaterialResources(material);
}

}  // namespace nova::gfx
