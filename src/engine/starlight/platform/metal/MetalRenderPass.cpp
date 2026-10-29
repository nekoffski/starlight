#include "MetalRenderPass.hh"

#include <algorithm>

namespace sl {
namespace {

Result<u32> claimShaderBufferBinding(u64 binding, u32 bindingMask) {
    if (binding >= metalVertexBufferIndex) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Shader buffer binding {} is reserved or out of range", binding
        );
    }

    const u32 bindingBit = 1u << binding;
    if (bindingMask & bindingBit) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Shader buffer binding {} is already in use", binding
        );
    }

    return bindingMask | bindingBit;
}

Result<u32> bindUniforms(
    MTL::RenderCommandEncoder& encoder, std::span<const ShaderUniform> uniforms,
    std::span<const ShaderParameterBlock> parameters
) {
    if (parameters.size() != uniforms.size()) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Shader parameter block count does not match reflection"
        );
    }

    u32 bindingMask = 0;
    for (const auto& uniform : uniforms) {
        const auto parameter = std::ranges::find(
            parameters, uniform.name, &ShaderParameterBlock::name
        );
        const bool matchingParameter = parameter != parameters.end() &&
                                       parameter->bytes.size() == uniform.size;
        const bool supportedSize =
            uniform.size > 0 && uniform.size <= metalMaxInlineBufferSize;

        if (not matchingParameter || not supportedSize) {
            return Error::unexpected(
                ErrorCode::invalidArgument,
                "Invalid shader parameter block: {}", uniform.name
            );
        }

        const auto claimed =
            claimShaderBufferBinding(uniform.binding, bindingMask);
        if (not claimed) {
            return Error::unexpected(claimed.error());
        }
        bindingMask = *claimed;

        encoder.setVertexBytes(
            parameter->bytes.data(), parameter->bytes.size(), uniform.binding
        );
        encoder.setFragmentBytes(
            parameter->bytes.data(), parameter->bytes.size(), uniform.binding
        );
    }
    return bindingMask;
}

Result<void> storeResourceId(
    Bytes& bytes, const ShaderResourceArray& array, u32 index,
    MTL::ResourceID id
) {
    if (index >= array.count) {
        return Error::unexpected(
            ErrorCode::invalidArgument,
            "Resource index exceeds shader table capacity"
        );
    }

    const auto offset = array.offset + index * array.stride;
    std::memcpy(bytes.data() + offset, &id, sizeof(id));
    return {};
}

Result<void> encodeResourceArray(
    Bytes& bytes, const ShaderResourceArray& array,
    MetalResourcePool& resourcePool,
    const ShaderResourceDependencies& dependencies,
    MTL::RenderCommandEncoder& encoder
) {
    if (array.kind == ShaderResourceKind::texture) {
        for (auto texture : dependencies.textures) {
            auto* resource = resourcePool.getTexture(texture);
            const auto stored = storeResourceId(
                bytes, array, texture.id, resource->gpuResourceID()
            );
            if (not stored) {
                return stored;
            }

            encoder.useResource(
                resource, MTL::ResourceUsageRead,
                MTL::RenderStageVertex | MTL::RenderStageFragment
            );
        }
    } else {
        for (auto sampler : dependencies.samplers) {
            auto* resource = resourcePool.getSampler(sampler);
            const auto stored = storeResourceId(
                bytes, array, sampler.id, resource->gpuResourceID()
            );
            if (not stored) {
                return stored;
            }
        }
    }
    return {};
}

Result<Bytes> buildResourceTable(
    const ShaderResourceTable& table, MetalResourcePool& resourcePool,
    const ShaderResourceDependencies& dependencies,
    MTL::RenderCommandEncoder& encoder
) {
    // Snapshot tables so new uploads cannot mutate in-flight bindings.
    Bytes bytes(table.size);
    for (const auto& array : table.arrays) {
        const bool supportedCount =
            array.count > 0 && array.count <= resourceTableCapacity;
        const bool validStride = array.stride == sizeof(MTL::ResourceID);
        const u64 arrayEnd =
            u64(array.offset) + u64(array.count) * array.stride;
        const bool withinTable = arrayEnd <= bytes.size();

        if (not supportedCount || not validStride || not withinTable) {
            return Error::unexpected(
                ErrorCode::invalidArgument,
                "Invalid Metal resource array layout"
            );
        }

        const auto encoded = encodeResourceArray(
            bytes, array, resourcePool, dependencies, encoder
        );
        if (not encoded) {
            return Error::unexpected(encoded.error());
        }
    }
    return bytes;
}

Result<void> bindResourceTables(
    MTL::RenderCommandEncoder& encoder, MetalResourcePool& resourcePool,
    std::span<const ShaderResourceTable> tables,
    const ShaderResourceDependencies& dependencies, u32 bindingMask
) {
    for (const auto& table : tables) {
        const bool supportedSize =
            table.size > 0 && table.size <= metalMaxInlineBufferSize;
        if (not supportedSize) {
            return Error::unexpected(
                ErrorCode::invalidArgument,
                "Unsupported Metal resource table layout"
            );
        }

        const auto claimed =
            claimShaderBufferBinding(table.binding, bindingMask);
        if (not claimed) {
            return Error::unexpected(claimed.error());
        }
        bindingMask = *claimed;

        const auto bytes =
            buildResourceTable(table, resourcePool, dependencies, encoder);
        if (not bytes) {
            return Error::unexpected(bytes.error());
        }

        encoder.setVertexBytes(bytes->data(), bytes->size(), table.binding);
        encoder.setFragmentBytes(bytes->data(), bytes->size(), table.binding);
    }
    return {};
}

}  // namespace

MetalRenderPassEncoder::MetalRenderPassEncoder(
    MetalResourcePool& resourcePool, MTL::RenderCommandEncoder* encoder
)
    : m_resourcePool(resourcePool), m_encoder(encoder) {}

MetalRenderPassEncoder::~MetalRenderPassEncoder() { m_encoder->endEncoding(); }

Result<void> MetalRenderPassEncoder::setPipeline(
    GraphicsPipelineHandle handle
) {
    auto* pipeline = m_resourcePool.getGraphicsPipeline(handle);

    if (not pipeline) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid graphics pipeline handle"
        );
    }

    m_encoder->setRenderPipelineState(&pipeline->state());
    return {};
}

Result<void> MetalRenderPassEncoder::setShaderParameters(
    const ShaderDescription& description,
    std::span<const ShaderParameterBlock> parameters,
    const ShaderResourceDependencies& dependencies
) {
    const auto validation = m_resourcePool.validateResources(dependencies);
    if (not validation) {
        return validation;
    }

    const auto bindingMask =
        bindUniforms(*m_encoder, description.uniforms, parameters);
    if (not bindingMask) {
        return Error::unexpected(bindingMask.error());
    }

    return bindResourceTables(
        *m_encoder, m_resourcePool, description.resourceTables, dependencies,
        *bindingMask
    );
}

void MetalRenderPassEncoder::draw(u32 vertexCount) {
    m_encoder->drawPrimitives(
        MTL::PrimitiveType::PrimitiveTypeTriangle, 0u, vertexCount, 1u
    );
}

void MetalRenderPassEncoder::drawIndexed(u32 indexCount, u32 instanceCount) {
    u64 indexBufferOffset = 0u;
    MTL::Buffer* indexBuffer = nullptr;

    if (m_indexBuffer) {
        indexBuffer = &m_indexBuffer->buffer->handle();
        indexBufferOffset = m_indexBuffer->offset;
    }

    m_encoder->drawIndexedPrimitives(
        MTL::PrimitiveType::PrimitiveTypeTriangle, indexCount,
        MTL::IndexType::IndexTypeUInt32, indexBuffer, indexBufferOffset,
        instanceCount
    );
}

Result<void> MetalRenderPassEncoder::setVertexBuffer(
    const DeviceBufferSlice& slice
) {
    auto* vertexBuffer = m_resourcePool.getBuffer(slice.handle);

    if (not vertexBuffer) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid vertex buffer handle"
        );
    }

    m_encoder->setVertexBuffer(
        &vertexBuffer->handle(), slice.offset, metalVertexBufferIndex
    );
    return {};
}

Result<void> MetalRenderPassEncoder::setIndexBuffer(
    const DeviceBufferSlice& slice
) {
    auto* indexBuffer = m_resourcePool.getBuffer(slice.handle);

    if (not indexBuffer) {
        return Error::unexpected(
            ErrorCode::invalidArgument, "Invalid index buffer handle"
        );
    }

    m_indexBuffer.emplace(indexBuffer, slice.offset);
    return {};
}

MTL::LoadAction toMetal(LoadOp op) {
    switch (op) {
        case LoadOp::load:
            return MTL::LoadActionLoad;
        case LoadOp::clear:
            return MTL::LoadActionClear;
        default:
            return MTL::LoadActionDontCare;
    }
}

MTL::StoreAction toMetal(StoreOp op) {
    switch (op) {
        case StoreOp::store:
            return MTL::StoreActionStore;
        default:
            return MTL::StoreActionDontCare;
    }
}

}  // namespace sl
