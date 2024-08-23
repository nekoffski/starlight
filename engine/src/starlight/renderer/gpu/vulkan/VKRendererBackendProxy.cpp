#include "VKRendererBackendProxy.hh"

#include "VKRendererBackend.hh"
#include "VKCommandBuffer.hh"

namespace sl::vk {

VKRendererBackendProxy::VKRendererBackendProxy(VKRendererBackend* backend) :
    m_backend(backend) {}

void VKRendererBackendProxy::drawMesh(const Mesh& mesh) {
    m_backend->drawMesh(mesh);
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

void VKRendererBackendProxy::setViewport(const Rect2<u32>& viewport) {
    m_backend->setViewport(viewport);
}

void VKRendererBackendProxy::gpuCall(std::function<void(CommandBuffer&)>&& callback
) {
    m_backend->gpuCall(std::move(callback));
}

UniqPtr<UIRenderer> VKRendererBackendProxy::createUIRendererer(RenderPass* renderPass
) {
    return m_backend->createUIRendererer(renderPass);
}

}  // namespace sl::vk