#pragma once

#include "starlight/core/utils/ResourcePool.hpp"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "VKTexture.h"
#include "VKShader.h"
#include "VKMesh.h"
#include "VKContext.h"
#include "VKPhysicalDevice.h"
#include "VKContext.h"
#include "VKBackendAccessor.h"
#include "VKRenderPass.h"
#include "VKRenderTarget.h"
#include "VKSwapchain.h"

#include "fwd.h"

namespace sl::vk {

class VKResourcePools : public ResourcePools {
public:
    explicit VKResourcePools(
      VKBackendAccessor& backendAccessor, VKBuffer& vertexBuffer,
      VKBuffer& indexBuffer, VKSwapchain& swapchain, VKRendererBackend* backend
    );

    VKMesh* createMesh(
      const Mesh::Properties& props, std::span<const Vertex3> vertices,
      std::span<const u32> indices, const Extent3& extent
    ) override;
    VKMesh* createMesh(
      const Mesh::Properties& props, std::span<const Vertex2> vertices,
      std::span<const u32> indices, const Extent2& extent
    ) override;
    void destroyMesh(Mesh& mesh) override;

    VKTexture* createTexture(
      const Texture::Properties& props, const std::span<u8> pixels
    ) override;
    void destroyTexture(Texture& texture) override;

    VKShader* createShader(const Shader::Properties& props) override;
    void destroyShader(Shader& shader) override;

    VKRenderTarget* createRenderTarget(
      const RenderTarget::Properties& props, RenderPass* renderPass
    ) override;
    void destroyRenderTarget(RenderTarget& renderTarget) override;

    VKRenderPass* createRenderPass(const RenderPass::Properties& props) override;
    void destroyRenderPass(RenderPass& renderPass) override;

private:
    VKBackendAccessor& m_backendAccessor;
    VKContext& m_context;
    VKLogicalDevice& m_device;
    VKBuffer& m_vertexBuffer;
    VKBuffer& m_indexBuffer;
    VKSwapchain& m_swapchain;

    VKRendererBackend* backend;  // TODO: temporary, remove when render pass creation
                                 // will be managed by this class, for now we need
                                 // this for render pass id

    ResourcePool<VKTexture> m_textures;
    ResourcePool<VKShader> m_shaders;
    ResourcePool<VKMesh> m_meshes;
    ResourcePool<VKRenderTarget> m_renderTargets;
    ResourcePool<VKRenderPass> m_renderPasses;
};

}  // namespace sl::vk