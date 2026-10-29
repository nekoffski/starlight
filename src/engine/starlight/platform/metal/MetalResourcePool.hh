#pragma once

#include <array>
#include <unordered_map>

#include "Metal.hh"
#include "MetalBuffer.hh"
#include "MetalGraphicsPipeline.hh"
#include "MetalRenderSurfaceProvider.hh"
#include "MetalShader.hh"
#include "starlight/core/Concepts.hh"
#include "starlight/core/Config.hh"
#include "starlight/core/Id.hh"
#include "starlight/renderer/rhi/RenderDevice.hh"

namespace sl {

class MetalResourcePool : public NonCopyable, public NonMovable {
    struct SurfaceWrapper {
        CA::MetalLayer* surface;
        std::shared_ptr<MetalRenderSurfaceProvider> provider;
    };

   public:
    explicit MetalResourcePool(const Config& config, MetalContext& ctx);

    Result<SurfaceHandle> attachSurface(
        std::shared_ptr<RenderSurfaceProvider> provider
    );

    CA::MetalLayer* getSurface(SurfaceHandle handle);
    void destroySurface(SurfaceHandle handle);

    Result<ShaderHandle> createShader(const ShaderDescription& description);
    void destroyShader(ShaderHandle handle);

    auto getShader(this auto&& self, ShaderHandle handle) {
        auto it = self.m_shaders.find(handle);
        return it != self.m_shaders.end() ? &*it->second : nullptr;
    }

    Result<GraphicsPipelineHandle> createGraphicsPipeline(
        const GraphicsPipelineDescription& description
    );
    void destroyGraphicsPipeline(GraphicsPipelineHandle handle);
    MetalGraphicsPipeline* getGraphicsPipeline(GraphicsPipelineHandle handle);

    Result<DeviceBufferHandle> createBuffer(
        const DeviceBufferDescription& description
    );
    void destroyBuffer(DeviceBufferHandle handle);
    MetalBuffer* getBuffer(DeviceBufferHandle handle);

    Result<TextureHandle> createTexture(const TextureUploadData& data);
    void destroyTexture(TextureHandle handle);
    MTL::Texture* getTexture(TextureHandle handle) const;

    Result<SamplerHandle> createSampler(const SamplerDescription& description);
    void destroySampler(SamplerHandle handle);
    MTL::SamplerState* getSampler(SamplerHandle handle) const;

    Result<void> validateResources(
        const ShaderResourceDependencies& resources
    ) const;

   private:
    template <typename T>
    struct ResourceEntry {
        NS::SharedPtr<T> resource;
        u32 generation{0};
    };

    using TextureTable =
        std::array<ResourceEntry<MTL::Texture>, resourceTableCapacity>;
    using SamplerTable =
        std::array<ResourceEntry<MTL::SamplerState>, resourceTableCapacity>;

    TextureTable m_textures;
    SamplerTable m_samplers;
    u32 m_maxTextures;

    IdLake m_idLake;
    MetalContext& m_ctx;

    std::unordered_map<SurfaceHandle, SurfaceWrapper> m_surfaces;
    std::unordered_map<ShaderHandle, std::unique_ptr<MetalShader>> m_shaders;
    std::unordered_map<
        GraphicsPipelineHandle, std::unique_ptr<MetalGraphicsPipeline>>
        m_graphicsPipelines;
    std::unordered_map<DeviceBufferHandle, std::unique_ptr<MetalBuffer>>
        m_buffers;
};

}  // namespace sl
