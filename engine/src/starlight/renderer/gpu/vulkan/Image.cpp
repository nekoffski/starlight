#include "Image.h"

#include "Device.h"

namespace sl::vk {

Image::Image(const Device* device, const Context* context, const Image::Properties& properties)
    : m_size(properties.size), m_device(device), m_context(context) {
    auto logicalDevice = m_device->getLogicalDevice();
    auto allocator     = m_context->getAllocator();

    createImage(properties, logicalDevice, allocator);
    allocateAndBindMemory(properties, allocator);

    if (properties.createView) createView(properties, logicalDevice, allocator);
}

Image::~Image() {
    auto logicalDevice = m_device->getLogicalDevice();
    auto allocator     = m_context->getAllocator();

    vkDestroyImageView(logicalDevice, m_view, allocator);
    vkFreeMemory(logicalDevice, m_memory, allocator);
    vkDestroyImage(logicalDevice, m_handle, allocator);

    LOG_TRACE("Image destroyed");
}

void Image::copyFromBuffer(Buffer& buffer, CommandBuffer& commandBuffer) {
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

void Image::transitionLayout(
    CommandBuffer& commandBuffer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout
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

    VkPipelineStageFlags source;
    VkPipelineStageFlags destination;

    // Don't care about the old layout - transition to optimal layout (for the underlying
    // implementation).
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        // Don't care what stage the pipeline is in at the start.
        source = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

        // Used for copying
        destination = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        // Transitioning from a transfer destination layout to a shader-readonly layout.
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        // From a copying stage to...
        source = VK_PIPELINE_STAGE_TRANSFER_BIT;

        // The fragment stage.
        destination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        LOG_FATAL("Unsupported layout transition");
        return;
    }

    vkCmdPipelineBarrier(
        commandBuffer.getHandle(), source, destination, 0, 0, 0, 0, 0, 1, &barrier
    );
}

VkImageCreateInfo createImageCreateInfo(const Image::Properties& properties) {
    VkImageCreateInfo imageCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    imageCreateInfo.imageType         = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width      = properties.size.width;
    imageCreateInfo.extent.height     = properties.size.height;
    imageCreateInfo.extent.depth      = 1;  // TODO: Support configurable depth.
    imageCreateInfo.mipLevels         = 4;  // TODO: Support mip mapping
    imageCreateInfo.arrayLayers       = 1;  // TODO: Support number of layers in the image.
    imageCreateInfo.format            = properties.format;
    imageCreateInfo.tiling            = properties.tiling;
    imageCreateInfo.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage             = properties.usage;
    imageCreateInfo.samples           = VK_SAMPLE_COUNT_1_BIT;  // TODO: Configurable sample count.
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;    // TODO: Configurable sharing mode.

    return imageCreateInfo;
}

VkImageViewCreateInfo createViewCreateInfo(
    const Image::Properties& properties, VkImage imageHandle
) {
    VkImageViewCreateInfo viewCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};

    viewCreateInfo.image                       = imageHandle;
    viewCreateInfo.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
    viewCreateInfo.format                      = properties.format;
    viewCreateInfo.subresourceRange.aspectMask = properties.viewAspectFlags;

    viewCreateInfo.subresourceRange.baseMipLevel   = 0;
    viewCreateInfo.subresourceRange.levelCount     = 1;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.layerCount     = 1;

    return viewCreateInfo;
}

VkMemoryRequirements getMemoryRequirements(VkDevice device, VkImage handle) {
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(device, handle, &memoryRequirements);

    return memoryRequirements;
}

VkMemoryAllocateInfo createMemoryAllocateInfo(
    const VkMemoryRequirements& requirements, uint32_t memoryTypeIndex
) {
    VkMemoryAllocateInfo memoryAllocateInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    memoryAllocateInfo.allocationSize       = requirements.size;
    memoryAllocateInfo.memoryTypeIndex      = memoryTypeIndex;

    return memoryAllocateInfo;
}

VkImageView Image::getView() { return m_view; }

void Image::createImage(
    const Properties& properties, VkDevice logicalDevice, VkAllocator allocator
) {
    auto imageCreateInfo = createImageCreateInfo(properties);
    VK_ASSERT(vkCreateImage(logicalDevice, &imageCreateInfo, allocator, &m_handle));
    LOG_TRACE("Image created");
}

void Image::allocateAndBindMemory(const Properties& properties, VkAllocator allocator) {
    auto logicalDevice = m_device->getLogicalDevice();

    auto memoryRequirements = getMemoryRequirements(logicalDevice, m_handle);
    auto memoryType =
        m_device->findMemoryIndex(memoryRequirements.memoryTypeBits, properties.memoryFlags);

    if (not memoryType) LOG_ERROR("Required memory type not found. Image not valid.");

    auto memoryAllocateInfo = createMemoryAllocateInfo(memoryRequirements, memoryType.value_or(-1));
    VK_ASSERT(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, allocator, &m_memory));

    VK_ASSERT(vkBindImageMemory(logicalDevice, m_handle, m_memory, 0));
}

void Image::createView(
    const Properties& properties, VkDevice logicalDevice, VkAllocator allocator
) {
    auto viewCreateInfo = createViewCreateInfo(properties, m_handle);
    VK_ASSERT(vkCreateImageView(logicalDevice, &viewCreateInfo, allocator, &m_view));
}

}  // namespace sl::vk
