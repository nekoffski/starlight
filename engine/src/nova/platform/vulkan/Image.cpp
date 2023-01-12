#include "Image.h"

#include "Device.h"

namespace nova::platform::vulkan {

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

    if (m_view) vkDestroyImageView(logicalDevice, m_view, allocator);
    if (m_memory) vkFreeMemory(logicalDevice, m_memory, allocator);
    if (m_handle) vkDestroyImage(logicalDevice, m_handle, allocator);
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
    LOG_INFO("Image created");
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

}  // namespace nova::platform::vulkan
