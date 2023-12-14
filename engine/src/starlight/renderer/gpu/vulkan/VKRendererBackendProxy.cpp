#include "VKRendererBackendProxy.h"

#include "VKRendererBackend.h"
#include "VKCommandBuffer.h"

namespace sl::vk {

VKRendererBackendProxy::VKRendererBackendProxy(VKRendererBackend* backend) :
    m_backend(backend) {}

void VKRendererBackendProxy::drawGeometry(const GeometryRenderData& renderData) {
    m_backend->drawGeometry(renderData);
}

VKCommandBuffer* VKRendererBackendProxy::getCommandBuffer() {
    return m_backend->getCommandBuffer();
}

u32 VKRendererBackendProxy::getImageIndex() { return m_backend->getImageIndex(); }

}  // namespace sl::vk