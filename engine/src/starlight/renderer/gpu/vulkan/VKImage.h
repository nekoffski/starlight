#pragma once

#include <optional>

#include "starlight/core/math/Size.hpp"

#include "Vulkan.h"
#include "fwd.h"

#include "VKBuffer.h"
#include "VKCommandBuffer.h"
#include "VKDevice.h"

namespace sl::vk {

class VKImage {
public:
    struct Properties {
        VkImageType type;
        Size2u32 size;
        VkFormat format;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlags memoryFlags;
        bool createView;
        VkImageAspectFlags viewAspectFlags;
    };

    explicit VKImage(
      const VKDevice* device, const VKContext* context, const Properties& properties
    );

    void copyFromBuffer(VKBuffer& buffer, VKCommandBuffer& commandBuffer);

    void transitionLayout(
      VKCommandBuffer& commandBuffer, VkFormat format, VkImageLayout oldLayout,
      VkImageLayout newLayout
    );

    ~VKImage();

    VkImageView getView() const;

private:
    void createImage(
      const Properties& properties, VkDevice logicalDevice, VkAllocator allocator
    );
    void allocateAndBindMemory(const Properties& properties, VkAllocator allocator);
    void createView(
      const Properties& properties, VkDevice logicalDevice, VkAllocator allocator
    );

    VkImage m_handle;
    VkDeviceMemory m_memory;
    VkImageView m_view;
    Size2u32 m_size;

    const VKDevice* m_device;
    const VKContext* m_context;
};

}  // namespace sl::vk
