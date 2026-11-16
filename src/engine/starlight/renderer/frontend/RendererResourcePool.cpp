#include "RendererResourcePool.hh"

#include <algorithm>
#include <array>
#include <cstring>
#include <kcore/Functional.hh>
#include <kcore/Pipeline.hh>

namespace sl {
namespace {

bool isValidVertexType(ValueType type) {
    constexpr std::array allowedTypes{
        ValueType::f32_2x1,
        ValueType::f32_3x1,
        ValueType::f32_4x1,
    };
    return std::ranges::find(allowedTypes, type) != allowedTypes.end();
}

Result<void> validatePrimitiveLayout(const PrimitiveUploadData& data) {
    constexpr u32 verticesPerTriangle = 3;
    const bool validCounts = data.vertexCount > 0 && data.indexCount > 0 &&
                             data.indexCount % verticesPerTriangle == 0;
    if (data.vertexLayout.empty() || not validCounts) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Geometry requires a vertex layout, vertices, and triangle indices"
        );
    }
    for (const auto type : data.vertexLayout) {
        if (not isValidVertexType(type)) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Unsupported geometry vertex type"
            );
        }
    }
    return {};
}

Result<void> validatePrimitiveByteCounts(const PrimitiveUploadData& data) {
    const auto stride = LayoutHelper::calculateStride(data.vertexLayout);
    const bool packedVertices =
        data.vertexData.size() % data.vertexCount == 0 &&
        data.vertexData.size() / data.vertexCount == stride;
    const bool packedIndices =
        data.indexData.size() == u64(data.indexCount) * sizeof(u32);
    if (not packedVertices || not packedIndices) {
        return Error::unexpected(
            kc::ErrorCode::invalidArgument,
            "Geometry byte counts do not match its layout and counts"
        );
    }
    return {};
}

Result<void> validatePrimitiveIndices(const PrimitiveUploadData& data) {
    for (u32 i = 0; i < data.indexCount; ++i) {
        u32 index;
        std::memcpy(
            &index, data.indexData.data() + size_t(i) * sizeof(index),
            sizeof(index)
        );
        if (index >= data.vertexCount) {
            return Error::unexpected(
                kc::ErrorCode::invalidArgument,
                "Geometry index exceeds vertex count"
            );
        }
    }
    return {};
}

Result<void> validatePrimitiveUpload(const PrimitiveUploadData& data) {
    return kc::startPipeline([&] { return validatePrimitiveLayout(data); })
        .then([&] { return validatePrimitiveByteCounts(data); })
        .then([&] { return validatePrimitiveIndices(data); })
        .result();
}

}  // namespace

RendererResourcePool::RendererResourcePool(
    const Config& config, RenderDevice& device
)
    : m_config(config), m_device(device) {}

Result<PrimitiveHandle> RendererResourcePool::createPrimitive(
    const PrimitiveUploadData& description
) {
    const auto r = validatePrimitiveUpload(description);
    if (not r) {
        return Error::unexpected(r.error());
    }
    DeviceBufferDescription vertexBufferDescription{
        .usage = DeviceBufferUsage::vertexBuffer,
        .bytes = description.vertexData
    };

    auto vertexBuffer = m_device.createBuffer(vertexBufferDescription);

    if (not vertexBuffer) {
        return Error::unexpected(vertexBuffer.error());
    }
    kc::DeferCall releaseVertex{[&] { m_device.destroyBuffer(*vertexBuffer); }};

    DeviceBufferDescription indexBufferDescription{
        .usage = DeviceBufferUsage::indexBuffer,
        .bytes = description.indexData,
    };

    auto indexBuffer = m_device.createBuffer(indexBufferDescription);

    if (not indexBuffer) {
        return Error::unexpected(indexBuffer.error());
    }
    kc::DeferCall releaseIndex{[&] { m_device.destroyBuffer(*indexBuffer); }};

    PrimitiveHandle handle{m_idAllocator.acquire<Primitive>()};

    Primitive primitive{
        .vertexLayout = description.vertexLayout,
        .vertexBuffer = *vertexBuffer,
        .indexBuffer = *indexBuffer,
        .vertexBufferOffset = 0,
        .indexBufferOffset = 0,
        .indexCount = description.indexCount
    };
    m_primitives.emplace(handle, std::move(primitive));
    releaseIndex.dismiss();
    releaseVertex.dismiss();

    return handle;
}

void RendererResourcePool::destroyPrimitive(PrimitiveHandle handle) {
    if (const auto found = m_primitives.find(handle);
        found != m_primitives.end()) {
        m_device.destroyBuffer(found->second.vertexBuffer);
        m_device.destroyBuffer(found->second.indexBuffer);
        m_primitives.erase(found);
    }
}

Primitive* RendererResourcePool::getPrimitive(PrimitiveHandle handle) {
    if (auto it = m_primitives.find(handle); it != m_primitives.end()) {
        return &it->second;
    }
    return nullptr;
}

}  // namespace sl
