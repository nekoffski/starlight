#include "VKResourcePools.hh"

#include "VKRendererBackend.hh"

namespace sl::vk {

VKResourcePools::VKResourcePools(
  VKContext& context, VKLogicalDevice& device, VKBuffer& vertexBuffer,
  VKBuffer& indexBuffer, VKSwapchain& swapchain, VKRendererBackend* backend
) :
    m_context(context),
    m_device(device), m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer),
    m_swapchain(swapchain), backend(backend), m_renderTargets("RenderTarget", 64),
    m_renderPasses("RenderPass", 64) {
    LOG_TRACE("VKResourcePools created");
}

VKRenderTarget* VKResourcePools::createRenderTarget(
  const RenderTarget::Properties& props, RenderPass* renderPass
) {
    // return m_renderTargets.create(
    //   m_context, m_device, static_cast<VKRenderPass*>(renderPass), props
    // );
}
void VKResourcePools::destroyRenderTarget(RenderTarget& renderTarget) {
    // m_renderTargets.destroy(renderTarget.getId());
}

VKRenderPass* VKResourcePools::createRenderPass(const RenderPass::Properties& props
) {
    // return m_renderPasses.create(m_context, m_device, m_swapchain, props);
}

void VKResourcePools::destroyRenderPass(RenderPass& renderPass) {
    // m_renderPasses.destroy(renderPass.getId());
}

}  // namespace sl::vk
