#include "VKMesh.h"

namespace sl::vk {

VKMesh::VKMesh(
  u32 id, VKBackendAccessor& backendAccessor, VKBuffer& vertexBuffer,
  VKBuffer& indexBuffer, const Properties& props, const Data& data
) :
    Mesh(props, data, id),
    m_backendAccessor(backendAccessor), m_context(*backendAccessor.getContext()),
    m_device(*backendAccessor.getLogicalDevice()) {
    LOG_TRACE("Creating Mesh");
    LOG_DEBUG(
      "Mesh data vertexSize={}, vertexCount={}, "
      "indicesCount={}, vertexDataPtr={}",
      data.vertexSize, data.vertexCount, data.indices.size(), data.vertexData
    );

    m_dataDescription.vertexCount        = data.vertexCount;
    m_dataDescription.vertexElementSize  = data.vertexSize;
    m_dataDescription.indexCount         = data.indices.size();
    m_dataDescription.indexElementSize   = sizeof(u32);
    m_dataDescription.vertexBufferOffset = 0;
    m_dataDescription.indexBufferOffset  = 0;

    upload(vertexBuffer, indexBuffer, data);
    LOG_TRACE("Mesh created");
}

VKMesh::~VKMesh() {
    LOG_TRACE("Destroying mesh");
    m_device.waitIdle();
}

u64 VKMesh::uploadDataRange(
  VkCommandPool pool, VkFence fence, VkQueue queue, VKBuffer& outBuffer, u64 size,
  const void* data
) {
    // TODO: shouldn't it be a part of VKBuffer class?
    const auto offset = outBuffer.allocate(size);

    VkMemoryPropertyFlags flags =
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VKBuffer stagingBuffer(
      m_backendAccessor,
      VKBuffer::Properties{ size, flags, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, true }
    );

    stagingBuffer.loadData(0, size, 0, data);
    stagingBuffer.copyTo(
      pool, fence, queue, outBuffer.getHandle(),
      VkBufferCopy{ .srcOffset = 0, .dstOffset = offset, .size = size }
    );
    // this should return optional in case if allocate returns false
    return offset;
}

void VKMesh::upload(
  VKBuffer& vertexBuffer, VKBuffer& indexBuffer, const Data& data
) {
    auto pool  = m_device.getGraphicsCommandPool();
    auto queue = m_device.getQueues().graphics;

    m_dataDescription.vertexBufferOffset = uploadDataRange(
      pool, nullptr, queue, vertexBuffer, m_dataDescription.verticesTotalSize(),
      data.vertexData
    );

    // TODO: allow for indices to be optional
    if (m_dataDescription.indexCount > 0) {
        m_dataDescription.indexBufferOffset = uploadDataRange(
          pool, nullptr, queue, indexBuffer, m_dataDescription.indicesTotalSize(),
          data.indices.data()
        );
    }
}

}  // namespace sl::vk