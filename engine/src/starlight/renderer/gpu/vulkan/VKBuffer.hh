#pragma once

#include <cstdint>

#include <kc/core/Countable.hpp>

#include "starlight/core/memory/Memory.hh"
#include "starlight/core/utils/FreeList.hh"

#include "VKLogicalDevice.hh"
#include "VKContext.hh"

#include "Vulkan.hh"
#include "fwd.hh"

namespace sl::vk {

class VKBuffer : public kc::core::Countable<VKBuffer> {
public:
    struct Properties {
        uint64_t size;
        VkMemoryPropertyFlags memoryPropertyFlags;
        VkBufferUsageFlags usageFlags;

        bool bindOnCreate;
        bool useFreeList = true;
    };

    explicit VKBuffer(
      VKContext& context, VKLogicalDevice& device, const Properties& props
    );
    ~VKBuffer();

    VkBuffer getHandle() const { return m_handle; }

    VkBuffer* getHandlePointer() { return &m_handle; }

    void destroy();

    void bind(uint64_t offset);
    bool resize(uint64_t size, VkQueue queue, VkCommandPool pool);
    void* lockMemory(uint64_t offset, uint64_t size, VkMemoryPropertyFlags flags);

    uint64_t allocate(uint64_t size);
    void free(uint64_t size, uint64_t offset);

    void unlockMemory();

    void loadData(
      uint64_t offset, uint64_t size, VkMemoryPropertyFlags flags, const void* data
    );

    void copyTo(
      VkCommandPool pool, VkFence fence, VkQueue queue, VkBuffer destination,
      const VkBufferCopy& copyRegion
    );

private:
    VkBufferCreateInfo createBufferCreateInfo() const;

    VkMemoryRequirements getMemoryRequirements(VkBuffer buffer) const;

    VkMemoryAllocateInfo createMemoryAllocateInfo(
      VkMemoryRequirements memoryRequirements
    ) const;

    VKContext& m_context;
    VKLogicalDevice& m_device;

    uint64_t m_totalSize;

    VkBuffer m_handle;
    VkBufferUsageFlags m_usageFlags;

    bool m_isLocked;

    VkDeviceMemory m_memory;

    int32_t m_memoryIndex;
    uint32_t m_memoryPropertyFlags;

    bool m_useFreeList;
    LocalPtr<FreeList> m_bufferFreeList;
};

}  // namespace sl::vk
