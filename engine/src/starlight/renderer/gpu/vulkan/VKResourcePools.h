#pragma once

#include "starlight/renderer/gpu/ResourcePool.hpp"
#include "starlight/renderer/gpu/ResourcePools.h"

#include "VKTexture.h"
#include "VKShader.h"
#include "VKGeometry.h"
#include "VKContext.h"
#include "VKDevice.h"
#include "VKRenderPass.h"
#include "VKRenderTarget.h"
#include "VKSwapchain.h"

#include "fwd.h"

namespace sl::vk {

class VKResourcePools : public ResourcePools {
public:
    explicit VKResourcePools(
      VKContext& context, VKDevice& device, VKBuffer& vertexBuffer,
      VKBuffer& indexBuffer, VKSwapchain& swapchain, VKRendererBackend* backend
    );

    VKGeometry* createGeometry(
      const Geometry::Properties& props, std::span<const Vertex3> vertices,
      std::span<const u32> indices, const Extent3& extent
    ) override;
    VKGeometry* createGeometry(
      const Geometry::Properties& props, std::span<const Vertex2> vertices,
      std::span<const u32> indices, const Extent2& extent
    ) override;
    void destroyGeometry(Geometry& geometry) override;

    VKTexture* createTexture(
      const Texture::Properties& props, const std::span<u8> pixels
    ) override;
    void destroyTexture(Texture& texture) override;

    VKTextureMap* createTextureMap(
      const TextureMap::Properties& props, Texture& texture
    ) override;
    void destroyTextureMap(TextureMap& textureMap) override;

    VKShader* createShader(const Shader::Properties& props) override;
    void destroyShader(Shader& shader) override;

    VKRenderTarget* createRenderTarget(
      const RenderTarget::Properties& props, RenderPass* renderPass
    ) override;
    void destroyRenderTarget(RenderTarget& renderTarget) override;

    VKRenderPass* createRenderPass(const RenderPass::Properties& props) override;
    void destroyRenderPass(RenderPass& renderPass) override;

private:
    VKContext& m_context;
    VKDevice& m_device;
    VKBuffer& m_vertexBuffer;
    VKBuffer& m_indexBuffer;
    VKSwapchain& m_swapchain;

    VKRendererBackend* backend;  // TODO: temporary, remove when render pass creation
                                 // will be managed by this class, for now we need
                                 // this for render pass id

    ResourcePool<VKTexture> m_textures;
    ResourcePool<VKTextureMap> m_textureMaps;
    ResourcePool<VKShader> m_shaders;
    ResourcePool<VKGeometry> m_geometries;
    ResourcePool<VKRenderTarget> m_renderTargets;
    ResourcePool<VKRenderPass> m_renderPasses;
};

}  // namespace sl::vk