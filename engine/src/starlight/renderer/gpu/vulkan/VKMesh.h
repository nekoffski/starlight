#pragma once

#include <span>

#include "VKDevice.h"
#include "VKBuffer.h"
#include "starlight/renderer/gpu/Mesh.h"

namespace sl::vk {

static constexpr int vulkanMaxMeshCount = 4096;

class VKMesh : public Mesh {
public:
    struct BufferData {};

    explicit VKMesh(
      u32 id, VKDevice* device, VKContext* context, VKBuffer& vertexBuffer,
      VKBuffer& indexBuffer, const Properties& props, const Data& data
    );
    ~VKMesh();

private:
    void upload(VKBuffer& vertexBuffer, VKBuffer& indexBuffer, const Data& data);

    u64 uploadDataRange(
      VkCommandPool pool, VkFence fence, VkQueue queue, VKBuffer& outBuffer,
      u64 size, const void* data
    );

    VKDevice* m_device;
    VKContext* m_context;
};

}  // namespace sl::vk
