#include "MetalResourcePool.hh"

namespace sl {

MetalResourcePool::MetalResourcePool(const Config& config, MetalContext& ctx)
    : m_maxTextures(
          std::min<u32>(config.renderer.maxTextures, resourceTableCapacity)
      ),
      m_ctx(ctx) {}

Result<SurfaceHandle> MetalResourcePool::attachSurface(
    std::shared_ptr<RenderSurfaceProvider> provider
) {
    auto metalProvider =
        std::dynamic_pointer_cast<MetalRenderSurfaceProvider>(provider);

    if (not metalProvider) {
        return Error::unexpected(
            ErrorCode::backendMismatch,
            "Provider is not a MetalRenderSurfaceProvider"
        );
    }

    auto layer = metalProvider->getLayer();
    layer->setDevice(&m_ctx.device());
    layer->setPixelFormat(
        MTL::PixelFormatBGRA8Unorm
    );  // TODO: make it configurable

    SurfaceHandle handle{m_idLake.acquire<SurfaceHandle>()};

    m_surfaces.emplace(
        std::make_pair(handle, SurfaceWrapper{layer, metalProvider})
    );

    return handle;
}

CA::MetalLayer* MetalResourcePool::getSurface(SurfaceHandle handle) {
    if (auto it = m_surfaces.find(handle); it != m_surfaces.end()) {
        return it->second.surface;
    }
    return nullptr;
}

void MetalResourcePool::destroySurface(SurfaceHandle handle) {
    m_surfaces.erase(handle);
}

Result<ShaderHandle> MetalResourcePool::createShader(
    const ShaderDescription& description
) {
    auto maybeShader = MetalShader::create(m_ctx, description);

    if (not maybeShader) {
        return Error::unexpected(maybeShader.error());
    }

    ShaderHandle handle{m_idLake.acquire<ShaderHandle>()};
    m_shaders.emplace(std::make_pair(handle, std::move(maybeShader.value())));
    return handle;
}

void MetalResourcePool::destroyShader(ShaderHandle handle) {
    m_shaders.erase(handle);
}

Result<GraphicsPipelineHandle> MetalResourcePool::createGraphicsPipeline(
    const GraphicsPipelineDescription& description
) {
    auto shader = getShader(description.shader);

    if (not shader) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid shader handle"
        );
    }

    auto maybePipeline =
        MetalGraphicsPipeline::create(m_ctx, *shader, description);

    if (not maybePipeline) {
        return Error::unexpected(maybePipeline.error());
    }

    GraphicsPipelineHandle handle{m_idLake.acquire<GraphicsPipelineHandle>()};
    m_graphicsPipelines.emplace(
        std::make_pair(handle, std::move(maybePipeline.value()))
    );
    return handle;
}

void MetalResourcePool::destroyGraphicsPipeline(GraphicsPipelineHandle handle) {
    m_graphicsPipelines.erase(handle);
}

MetalGraphicsPipeline* MetalResourcePool::getGraphicsPipeline(
    GraphicsPipelineHandle handle
) {
    if (auto it = m_graphicsPipelines.find(handle);
        it != m_graphicsPipelines.end()) {
        return it->second.get();
    }
    return nullptr;
}

Result<DeviceBufferHandle> MetalResourcePool::createBuffer(
    const DeviceBufferDescription& description
) {
    auto maybeBuffer = MetalBuffer::create(m_ctx, description);

    if (not maybeBuffer) {
        return Error::unexpected(maybeBuffer.error());
    }

    DeviceBufferHandle handle{m_idLake.acquire<DeviceBufferHandle>()};
    m_buffers.emplace(std::make_pair(handle, std::move(maybeBuffer.value())));
    return handle;
}

void MetalResourcePool::destroyBuffer(DeviceBufferHandle handle) {
    m_buffers.erase(handle);
}

MetalBuffer* MetalResourcePool::getBuffer(DeviceBufferHandle handle) {
    if (auto it = m_buffers.find(handle); it != m_buffers.end()) {
        return it->second.get();
    }
    return nullptr;
}

Result<TextureHandle> MetalResourcePool::createTexture(
    const TextureUploadData& data
) {
    auto pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());
    if (not m_ctx.device().supportsFamily(MTL::GPUFamilyMetal3)) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Indexed textures require a Metal 3 device"
        );
    }
    const bool validDimensions = data.width > 0 && data.height > 0 &&
                                 data.width <= maxTextureDimension2D &&
                                 data.height <= maxTextureDimension2D;
    const bool supportedFormat = data.format == TextureFormat::rgba8unorm;
    const u64 bytesPerRow = u64(data.width) * rgba8BytesPerPixel;
    const bool tightlyPackedPixels =
        validDimensions && data.pixels.size() == bytesPerRow * data.height;

    if (not validDimensions || not supportedFormat || not tightlyPackedPixels) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Expected a nonempty, tightly packed RGBA8 image (at most {} x {})",
            maxTextureDimension2D, maxTextureDimension2D
        );
    }
    for (u32 index = 0; index < m_maxTextures; ++index) {
        auto& entry = m_textures[index];
        if (entry.resource) {
            continue;
        }
        auto* descriptor = MTL::TextureDescriptor::texture2DDescriptor(
            MTL::PixelFormatRGBA8Unorm, data.width, data.height, false
        );
        const auto storageMode = m_ctx.device().hasUnifiedMemory()
                                     ? MTL::StorageModeShared
                                     : MTL::StorageModeManaged;
        descriptor->setStorageMode(storageMode);
        descriptor->setUsage(MTL::TextureUsageShaderRead);
        auto texture = NS::TransferPtr(m_ctx.device().newTexture(descriptor));
        if (not texture) {
            return Error::unexpected(
                ErrorCode::metalError, "Failed to create texture"
            );
        }
        texture->replaceRegion(
            MTL::Region(0, 0, data.width, data.height), 0, data.pixels.data(),
            bytesPerRow
        );
        entry.resource = std::move(texture);
        return TextureHandle{index, entry.generation};
    }
    return Error::unexpected(
        ErrorCode::invalidArgument, "Texture table is full"
    );
}

MTL::Texture* MetalResourcePool::getTexture(TextureHandle handle) const {
    if (handle.id >= m_textures.size()) {
        return nullptr;
    }
    const auto& entry = m_textures[handle.id];
    return handle.generation == entry.generation ? entry.resource.get()
                                                 : nullptr;
}

void MetalResourcePool::destroyTexture(TextureHandle handle) {
    if (getTexture(handle)) {
        m_textures[handle.id].resource.reset();
        ++m_textures[handle.id].generation;
    }
}

Result<SamplerHandle> MetalResourcePool::createSampler(
    const SamplerDescription& description
) {
    if (not m_ctx.device().supportsFamily(MTL::GPUFamilyMetal3)) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Indexed samplers require a Metal 3 device"
        );
    }
    const bool supportedFilter = description.filter == TextureFilter::nearest ||
                                 description.filter == TextureFilter::linear;
    const bool supportedAddressMode =
        description.addressMode == TextureAddressMode::repeat ||
        description.addressMode == TextureAddressMode::clampToEdge;

    if (not supportedFilter || not supportedAddressMode) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid sampler description"
        );
    }
    for (u32 index = 0; index < m_samplers.size(); ++index) {
        auto& entry = m_samplers[index];
        if (entry.resource) {
            continue;
        }
        auto descriptor =
            NS::TransferPtr(MTL::SamplerDescriptor::alloc()->init());
        const auto filter = description.filter == TextureFilter::nearest
                                ? MTL::SamplerMinMagFilterNearest
                                : MTL::SamplerMinMagFilterLinear;
        const auto address =
            description.addressMode == TextureAddressMode::repeat
                ? MTL::SamplerAddressModeRepeat
                : MTL::SamplerAddressModeClampToEdge;
        descriptor->setMinFilter(filter);
        descriptor->setMagFilter(filter);
        descriptor->setSAddressMode(address);
        descriptor->setTAddressMode(address);
        descriptor->setSupportArgumentBuffers(true);
        auto sampler =
            NS::TransferPtr(m_ctx.device().newSamplerState(descriptor.get()));
        if (not sampler) {
            return Error::unexpected(
                ErrorCode::metalError, "Failed to create sampler"
            );
        }
        entry.resource = std::move(sampler);
        return SamplerHandle{index, entry.generation};
    }
    return Error::unexpected(
        ErrorCode::invalidArgument, "Sampler table is full"
    );
}

MTL::SamplerState* MetalResourcePool::getSampler(SamplerHandle handle) const {
    if (handle.id >= m_samplers.size()) {
        return nullptr;
    }
    const auto& entry = m_samplers[handle.id];
    return handle.generation == entry.generation ? entry.resource.get()
                                                 : nullptr;
}

void MetalResourcePool::destroySampler(SamplerHandle handle) {
    if (getSampler(handle)) {
        m_samplers[handle.id].resource.reset();
        ++m_samplers[handle.id].generation;
    }
}

Result<void> MetalResourcePool::validateResources(
    const ShaderResourceDependencies& resources
) const {
    for (auto handle : resources.textures) {
        if (not getTexture(handle)) {
            return Error::unexpected(
                ErrorCode::invalidArgument, "Invalid texture dependency"
            );
        }
    }
    for (auto handle : resources.samplers) {
        if (not getSampler(handle)) {
            return Error::unexpected(
                ErrorCode::invalidArgument, "Invalid sampler dependency"
            );
        }
    }
    return {};
}

}  // namespace sl
