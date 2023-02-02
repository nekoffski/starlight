#pragma once

#include <optional>

#include "nova/math/Size.hpp"

#include "Vulkan.h"
#include "fwd.h"

#include "Buffer.h"
#include "CommandBuffer.h"
#include "Device.h"

namespace nova::platform::vulkan {

class Image {
   public:
    struct Properties {
        VkImageType type;
        math::Size2u32 size;
        VkFormat format;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlags memoryFlags;
        bool createView;
        VkImageAspectFlags viewAspectFlags;
    };

    explicit Image(const Device* device, const Context* context, const Properties& properties);

    void copyFromBuffer(Buffer& buffer, CommandBuffer& commandBuffer);

    void transitionLayout(
        CommandBuffer& commandBuffer, VkFormat format, VkImageLayout oldLayout,
        VkImageLayout newLayout
    );

    ~Image();

    VkImageView getView();

   private:
    void createImage(const Properties& properties, VkDevice logicalDevice, VkAllocator allocator);
    void allocateAndBindMemory(const Properties& properties, VkAllocator allocator);
    void createView(const Properties& properties, VkDevice logicalDevice, VkAllocator allocator);

    VkImage m_handle;
    VkDeviceMemory m_memory;
    VkImageView m_view;
    math::Size2u32 m_size;

    const Device* m_device;
    const Context* m_context;
};

}  // namespace nova::platform::vulkan
