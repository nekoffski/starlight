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

    void copyFromBuffer(Buffer& buffer, CommandBuffer& commandBuffer) {
        VkBufferImageCopy region;
        zeroMemory(region);

        region.bufferOffset      = 0;
        region.bufferRowLength   = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel       = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount     = 1;

        region.imageExtent.width  = m_size.width;
        region.imageExtent.height = m_size.height;
        region.imageExtent.depth  = 1;

        vkCmdCopyBufferToImage(
            commandBuffer.getHandle(), buffer.getHandle(), m_handle,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region
        );
    }

    void transitionLayout(
        CommandBuffer& commandBuffer, VkFormat format, VkImageLayout oldLayout,
        VkImageLayout newLayout
    ) {
        auto queueFamilyIndex = m_device->getQueueIndices().graphics;

        VkImageMemoryBarrier barrier            = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        barrier.oldLayout                       = oldLayout;
        barrier.newLayout                       = newLayout;
        barrier.srcQueueFamilyIndex             = queueFamilyIndex;
        barrier.dstQueueFamilyIndex             = queueFamilyIndex;
        barrier.image                           = m_handle;
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;

        VkPipelineStageFlags source_stage;
        VkPipelineStageFlags dest_stage;

        // Don't care about the old layout - transition to optimal layout (for the underlying
        // implementation).
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
            newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            LOG_INFO("tu");
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            // Don't care what stage the pipeline is in at the start.
            source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

            // Used for copying
            dest_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            LOG_INFO("tu 2");
            // Transitioning from a transfer destination layout to a shader-readonly layout.
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            // From a copying stage to...
            source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;

            // The fragment stage.
            dest_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            // KFATAL("unsupported layout transition!");
            return;
        }

        LOG_INFO("tu 4");
        vkCmdPipelineBarrier(
            commandBuffer.getHandle(), source_stage, dest_stage, 0, 0, 0, 0, 0, 1, &barrier
        );
    }

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
