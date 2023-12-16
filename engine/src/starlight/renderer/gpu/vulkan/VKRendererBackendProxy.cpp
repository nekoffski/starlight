#include "VKRendererBackendProxy.h"

#include "VKRendererBackend.h"
#include "VKCommandBuffer.h"

namespace sl::vk {

VKRendererBackendProxy::VKRendererBackendProxy(VKRendererBackend* backend) :
    m_backend(backend) {}

void VKRendererBackendProxy::drawGeometry(const Geometry& geometry) {
    m_backend->drawGeometry(geometry);
}

VKCommandBuffer* VKRendererBackendProxy::getCommandBuffer() {
    return m_backend->getCommandBuffer();
}

u32 VKRendererBackendProxy::getImageIndex() { return m_backend->getImageIndex(); }

Texture* VKRendererBackendProxy::getFramebuffer(u64 id) {
    return m_backend->getFramebuffer(id);
}

Texture* VKRendererBackendProxy::getDepthBuffer() {
    return m_backend->getDepthBuffer();
}

}  // namespace sl::vk