#include "RendererResourcePool.hh"

namespace sl {

RendererResourcePool::RendererResourcePool(
    const Config& config, RenderDevice& device
)
    : m_config(config), m_device(device) {}

Result<PrimitiveHandle> RendererResourcePool::createPrimitive(
    const PrimitiveUploadData& description
) {
    DeviceBufferDescription vertexBufferDescription{
        .usage = DeviceBufferUsage::vertexBuffer,
        .bytes = description.vertexData
    };

    auto vertexBuffer = m_device.createBuffer(vertexBufferDescription);

    if (not vertexBuffer) {
        return Error::unexpected(
            ErrorCode::deviceOperationFailed,
            "Failed to create vertex buffer: {}", vertexBuffer.error()
        );
    }

    DeviceBufferDescription indexBufferDescription{
        .usage = DeviceBufferUsage::indexBuffer,
        .bytes = description.indexData,
    };

    auto indexBuffer = m_device.createBuffer(indexBufferDescription);

    if (not indexBuffer) {
        m_device.destroyBuffer(*vertexBuffer);

        return Error::unexpected(
            ErrorCode::deviceOperationFailed,
            "Failed to create index buffer: {}", indexBuffer.error()
        );
    }

    PrimitiveHandle handle{m_idLake.acquire<Primitive>()};

    Primitive primitive{
        .vertexBuffer = *vertexBuffer,
        .indexBuffer = *indexBuffer,
        .vertexBufferOffset = 0,
        .indexBufferOffset = 0,
        .indexCount = description.indexCount
    };
    m_primitives.emplace(handle, std::move(primitive));

    return handle;
}

void RendererResourcePool::destroyPrimitive(PrimitiveHandle handle) {
    if (auto primitive = getPrimitive(handle); primitive) {
        m_device.destroyBuffer(primitive->vertexBuffer);
        m_device.destroyBuffer(primitive->indexBuffer);
    }
}

Primitive* RendererResourcePool::getPrimitive(PrimitiveHandle handle) {
    if (auto it = m_primitives.find(handle); it != m_primitives.end()) {
        return &it->second;
    }
    return nullptr;
}

}  // namespace sl
