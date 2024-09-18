#pragma once

#include "starlight/core/ResourcePool.hh"
#include "starlight/renderer/gpu/ResourcePools.hh"

#include "VKTexture.hh"
#include "VKShader.hh"
#include "VKMesh.hh"
#include "VKContext.hh"
#include "VKPhysicalDevice.hh"
#include "VKContext.hh"

#include "VKRenderPass.hh"
#include "VKRenderTarget.hh"
#include "VKSwapchain.hh"

#include "fwd.hh"

namespace sl::vk {

class VKResourcePools : public ResourcePools {
public:
    explicit VKResourcePools(
      VKContext& context, VKLogicalDevice& device, VKBuffer& vertexBuffer,
      VKBuffer& indexBuffer, VKSwapchain& swapchain, VKRendererBackend* backend
    );

    VKRenderTarget* createRenderTarget(
      const RenderTarget::Properties& props, RenderPass* renderPass
    ) override;
    void destroyRenderTarget(RenderTarget& renderTarget) override;

    VKRenderPass* createRenderPass(const RenderPass::Properties& props) override;
    void destroyRenderPass(RenderPass& renderPass) override;

private:
    VKContext& m_context;
    VKLogicalDevice& m_device;
    VKBuffer& m_vertexBuffer;
    VKBuffer& m_indexBuffer;
    VKSwapchain& m_swapchain;

    VKRendererBackend* backend;  // TODO: temporary, remove when render pass creation
                                 // will be managed by this class, for now we need
                                 // this for render pass id

    ResourcePool<VKRenderTarget> m_renderTargets;
    ResourcePool<VKRenderPass> m_renderPasses;
};

}  // namespace sl::vk