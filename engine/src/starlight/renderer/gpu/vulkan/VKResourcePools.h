#pragma once

#include "starlight/renderer/ResourcePool.hpp"
#include "starlight/renderer/ResourcePools.h"

#include "VKTexture.h"
#include "VKShader.h"
#include "VKGeometry.h"
#include "VKContext.h"
#include "VKDevice.h"
#include "VKRendererContext.h"
#include "VKRenderPass.h"
#include "VKRenderTarget.h"
#include "VKSwapchain.h"

#include "fwd.h"

namespace sl::vk {

class VKResourcePools : public ResourcePools {
public:
    explicit VKResourcePools(
      VKContext& context, VKDevice& device, VKBuffer& vertexBuffer,
      VKBuffer& indexBuffer, VKRendererContext& rendererContext,
      VKSwapchain& swapchain, VKRendererBackend* backend
    );

    VKGeometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex3> vertices,
      const std::span<uint32_t> indices
    ) override;
    VKGeometry* createGeometry(
      const Geometry::Properties& props, const std::span<Vertex2> vertices,
      const std::span<uint32_t> indices
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

    VKRenderTarget* createRenderTarget(const RenderTarget::Properties& props
    ) override;
    void destroyRenderTarget(RenderTarget& renderTarget) override;

    VKRenderPass* createRenderPass(const RenderPass::Properties& props) override;
    void destroyRenderPass(RenderPass& renderPass) override;

private:
    VKContext& m_context;
    VKDevice& m_device;
    VKBuffer& m_vertexBuffer;
    VKBuffer& m_indexBuffer;
    VKRendererContext& m_rendererContext;
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